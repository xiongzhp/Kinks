angular.module('KinksApp')
.controller('viewController', function($scope, $location, $timeout, $cookies, jobService){
    if (!$cookies.user){
        $scope.registered = false;
        $scope.submitCode = function(){
            $location.path('/view/' + $scope.code);
        }
    }else{
        $scope.registered = true;
        var getlistRefresher;
        var getList = function(){
            jobService.list(
                $cookies.user,
                $cookies.pass
            ).success(function(res){
                $scope.jobs = res;
                var refresh = false;
                for (var i = 0; i < res.length; i ++){
                    if (res[i].status == 0){
                        refresh = true;
                        break;
                    }
                }
                if (refresh){
                    getlistRefresher = $timeout(getList, 2000);
                }
            }).error(function(res){
                noty({text: res.error, type:'error', timeout: 1000});
                $location.path('/');
            });
        }
        $scope.download = function(id){
            jobService.download(id).success(function(){
            }).error(function(){
                noty({text: 'Download error!', type:'error', timeout: 1000});
            });
        };
        $scope.$on('$locationChangeStart', function(e){
            $timeout.cancel(getlistRefresher);
        });
        getList();
    }
});