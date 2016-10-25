var mongoose = require('mongoose');

// create new user
exports.create_user = function create_user(userJSON, callback){
  var User = mongoose.model('User');
  var user = new User(userJSON);
  user.save(function(err){
    if (err) {
      console.log(err);
      callback('error');
    } else {
      console.log(userJSON);
      callback('success');
    }
  });
};

// unique username and email
exports.is_unique = function is_unique(username, email, callback){
  var User = mongoose.model('User');
  User.find({'$or':[{'username': username}, {'email': email}]}, function(err, results){
    if (err) {
      console.log(err);
      callback('error');
    } else if (results.length === 0) {
      callback('success');
    } else {
      callback('duplicate');
    }
  });
};

// unique username
exports.is_unique_username = function is_unique_username(username, callback){
  var User = mongoose.model('User');
  User.find({'username': username}, function(err, results){
    if (err) {
      console.log(err);
      callback('error');
    } else if (results.length === 0) {
      callback('success');
    } else {
      callback('duplicate');
    }
  });
};

// get salt and hash
exports.get_salt_hash = function get_salt_hash(username, callback){
  var User = mongoose.model('User');
  User.findOne({'username': username}, {'salt':1, 'hash':1, '_id':0}, function(err, result){
    if (err) {
      console.log(err);
      callback('error', '');
    } else if (result === null) {
      callback('fail', '');
    } else {
      callback('success', result);
    }    
  })
};

// login verify
exports.login_verify = function login_verify(username, hash, callback){
  var User = mongoose.model('User');
  User.find({'username': username, 'hash': hash}, function(err, results){
    if (err) {
      console.log(err);
      callback('error');
    } else if (results.length === 0) {
      callback('fail');
    } else {
      callback('success');
    }    
  });
};