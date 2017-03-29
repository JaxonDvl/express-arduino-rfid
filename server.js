/**
 * Created by Jaxxo on 22/01/2017.
 */
var express = require('express');
var app = express();
var bodyParser = require('body-parser');
var server = require('http').Server(app);
server.listen(3000);
var mongoose = require('mongoose');
var io = require('socket.io')(server);
/* mongo connect*/
mongoose.connect('mongodb://localhost:27017/ardunode');

var userSchema = mongoose.Schema({
    name: String,
    password: String,
    id: String
});
var User = mongoose.model('User', userSchema);

app.use(express.static('public'));
app.use(bodyParser.urlencoded({extended: true}));
app.use(bodyParser.json());
app.get('/', function (req, res) {
    res.send('Hello World!');

});
var SerialPort = require('serialport');
SerialPort.list(function (err, ports) {
    ports.forEach(function(port) {
        console.log(port.comName);
    });
});
var port = new SerialPort('COM6', {
    baudRate: 9600,
    parser: SerialPort.parsers.readline("\n")
});
port.on('open', function() {
    console.log('open');
});
function getUserAccount(userid) {
    return axios.get('/getuser?id='+userid)
        .then(function(response){
            if(response.data===''){

                return 0;
            } else{
                return 1;
            }
        });
}
io.on('connection', function (socket) {
    console.log('connected');
        port.on('data', function(data){
            console.log(data);

            port.write("B");
            if(data[0]==="L") {
                socket.emit('idscanned', {cardid: data});
            }
        });

});

app.post('/add_user',function(req,res){
    var currentUser = new User({name: req.body.name,password: req.body.password, id:req.body.id});
    currentUser.save(function (err, user) {
        if (err) return console.error(err);
    });
    console.log('added user');
});
app.get('/getuser', function (req, res) {
    var userid = req.param('id');

    User.findOne({'id':userid},function (err, users) {
        if (err) return console.error(err);
        res.send(users);
    })

});
