angular.module('KinksApp')
.run(function($anchorScroll) {
    $anchorScroll.yOffset = 100;
})
.controller('resultController', function($scope, $window, $routeParams, $cookies, $location, $anchorScroll, $timeout, jobService){

    var showResult = function(){
        jobService.preview($routeParams.id).success(function(res){
            $scope.forDownload = true;
            $scope.results = res.results;
            $scope.showDetail = false;
            // var filename = $routeParams.id + '.tar'
            // angular.element("#download").attr({
            //     'href': 'http://localhost:5555/'+filename
            // });
            // $scope.forDownload = true;
        });
    };

    var getProgressRefresher;
    var showProgress = function(){
        jobService.progress($routeParams.id).success(function(res){
            $scope.job_progress = res.progress;
            if (res.progress == 10){
                $timeout.cancel(getProgressRefresher);
                $window.location.reload();
            }
            getProgressRefresher = $timeout(showProgress, 2000);
        });
    }


    $scope.finished = false;
    $scope.error = false;
    $scope.progress = false;
    $scope.code = $routeParams.id;

    jobService.status($routeParams.id).success(function(res){
        if (res.status === undefined){
            noty({text: 'Wrong retrieval code!', type:'error', timeout: 1000});
            $location.path('/view');
        }else if (res.status == 2){
            $scope.finished = true;
            showResult();
        }else if (res.status == 1){
            $scope.error = true;
        }else{
            $scope.progress = true;
            showProgress();
        }
    });

    $scope.$on('$locationChangeStart', function(e){
        $timeout.cancel(getProgressRefresher);
    });

    $scope.download = function(id){
        jobService.download(id).success(function(){
        }).error(function(){
            noty({text: 'Download error!', type:'error', timeout: 1000});
        });
    };

});