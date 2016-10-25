angular.module('KinksApp')
.factory('captchaService', function($http){
    var captchaServiceAPI = {};

    captchaServiceAPI.create = function(){
        return $http.post('/captcha/new');
    };

    captchaServiceAPI.update = function(id){
        return $http.post('/captcha/update', {
            id:id 
        });
    };

    captchaServiceAPI.check_code = function(captcha_id, captcha_code){
        return $http.post('/captcha/check', {
            captcha_id: captcha_id,
            captcha_code: captcha_code
        });
    };


    return captchaServiceAPI;
});