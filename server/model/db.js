var mongoose = require('mongoose');

function toLower (v) {
  return v.toLowerCase();
}

var userSchema = new mongoose.Schema({
    username: String,
    email: {type: String, set: toLower},
    country: String,
    organization: String,
    salt: String,
    hash: String,
    date: {type: Date, default: Date.now}
});

var resultSchema = new mongoose.Schema({
    _id: mongoose.Schema.Types.ObjectId,
    chain: String,
    firstresi: String,
    lastresi: String,
    kinkresi: String,
    kinkang: Number,
    helix: String
});

var jobSchema = new mongoose.Schema({
    username: String,
    pdbid: String,
    status: {type: Number, default: 0}, // 0: new, 1: error, 2: done
    progress: {type: Number, default: 0},
    date: {type: Date, default: Date.now},
    results: [resultSchema]
});

var captchaSchema = new mongoose.Schema({
    code: Number
});

mongoose.model('User', userSchema);
mongoose.model('Result', resultSchema);
mongoose.model('Job', jobSchema);
mongoose.model('Captcha', captchaSchema);

mongoose.connect('mongodb://xiong:xiong@ds035014.mlab.com:35014/kinks');