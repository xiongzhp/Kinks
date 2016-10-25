angular.module('KinksApp')
.factory('jobService', function($http){
    var jobServiceAPI = {};

    jobServiceAPI.create = function(username, password, pdbid, captcha_id, captcha_code){
        return $http.post('/create', {
            username: username,
            password: password,
            pdbid: pdbid,
            captcha_id: captcha_id,
            captcha_code: captcha_code
        });
    };

    jobServiceAPI.createUnregistered = function(pdbid, captcha_id, captcha_code){
        return $http.post('/createUnregistered', {
            pdbid: pdbid,
            captcha_id: captcha_id,
            captcha_code: captcha_code
        });
    };

    jobServiceAPI.list = function(username, password){
        return $http.post('/list', {
            username: username,
            password: password
        });
    };

    jobServiceAPI.preview = function(id){
        return $http.post('/preview', {
            id: id
        });        
    };

    jobServiceAPI.details = function(job_id, result_index){
        return $http.post('/details', {
            job_id: job_id,
            result_index: result_index
        });        
    };

    jobServiceAPI.results = function(id){
        return $http.post('/results', {
            id: id
        });
    };

    jobServiceAPI.progress = function(id){
        return $http.post('/progress', {
            id: id
        });
    };

    jobServiceAPI.status = function(id){
        return $http.post('/status', {
            id: id
        });
    };


    return jobServiceAPI;
});
