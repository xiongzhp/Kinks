angular.module('KinksApp')
.factory('userService', function($http){
    var userServiceAPI = {};

    userServiceAPI.register = function(username, email, country, organization, password, verification){
        return $http.post('/signup', {
            username: username,
            email: email,
            country: country,
            organization: organization,
            password: password,
            verification: verification
        });
    };

    userServiceAPI.login = function(username, password){
        return $http.post('/signin', {
            username: username,
            password: password
        });
    };

    userServiceAPI.check_username = function(username){
        return $http.post('/signup/check/username', {
            username: username
        });
    };

    return userServiceAPI;
});