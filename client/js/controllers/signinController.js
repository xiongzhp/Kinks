angular.module('KinksApp')
.controller('signinController', function($scope, $rootScope, $location, $cookies, userService){
    $scope.submitForm = function(){
        userService.login(
            $scope.username,
            $scope.password
        ).success(function(){
            noty({text: 'Login successful!', type:'success', timeout: 1000});
            $cookies.user = $scope.username;
            $cookies.pass = $scope.password;
            $rootScope.user = $scope.username;
            $location.path('/');
        }).error(function(res){
            noty({text: res.error, type:'error', timeout: 1000});
        });
    }
});