angular.module('KinksApp')
.controller('indexController', function($scope, $rootScope, $cookies, $location){
	$rootScope.user = $cookies.user;
    $scope.logout = function(){
        noty({text: 'Signout successful!', type:'success', timeout: 1000});
        delete $cookies.user;
        delete $cookies.pass;
        $rootScope.user = null;
        $location.path('/');
        return false;
    };
});