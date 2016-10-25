var mongoose = require('mongoose');

exports.create_captcha = function create_captcha(number, callback){
    var Captcha = mongoose.model('Captcha');
    var captcha = new Captcha({
        code: number
    });
    captcha.save(function(err, captcha){
        if (err) {
          console.log(err);
          callback('error');
        } else {
          console.log(captcha);
          callback('success', captcha._id);
        }
    });
};

exports.update_captcha = function update_captcha(id, number, callback){
    var Captcha = mongoose.model('Captcha');
    Captcha.findOneAndUpdate({_id: id}, {code: number}, function(err){
        if (err) {
          callback('error');
        } else {
          callback('success');
        }   
    });
};

exports.get_code = function get_code(id, callback){
    var Captcha = mongoose.model('Captcha');
    Captcha.findById(id, function(err, result){
        if (err) {
            callback('error');
        } else {
            callback('success', result.code);
        }
    });
};

exports.get_code_and_remove = function get_code_and_remove(id, callback){
    var Captcha = mongoose.model('Captcha');
    Captcha.findById(id, function(err, result){
        if (err) {
            callback('error');
        } else {
            callback('success', result.code);
            result.remove();
        }
    });
};