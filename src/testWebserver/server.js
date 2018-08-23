const express = require('express')
const app = express()
const bodyParser = require('body-parser')
const port = 8080

var allowCrossDomain = function(req, res, next) {
    res.header('Access-Control-Allow-Origin', '*');
    res.header('Access-Control-Allow-Methods', 'GET,PUT,POST,DELETE,OPTIONS');
    res.header('Access-Control-Allow-Headers', 'Content-Type, Authorization, Content-Length, X-Requested-With, Accept');

    // intercept OPTIONS method
    if ('OPTIONS' == req.method) {
        res.send(200);
    } else {
        next();
    }
};
app.use(allowCrossDomain);

app.use('/', express.static('./'))
app.use(bodyParser.json()); // support json encoded bodies
app.use(bodyParser.urlencoded({ extended: true })); // support encoded bodies

app.get('/up', (req, res) => res.send('up'))
app.get('/down', (req, res) => res.send('down'))
app.get('/open', (req, res) => res.send('open'))
app.get('/close', (req, res) => res.send('closed'))
app.get('/stop', (req, res) => res.send('stopped'))

app.get('/connection', (req, res) => res.json({
    status : 'connected',
    openingTime : '05:30',
    closingTime : '21:00',
    openingDuration : 17,
    closingDuration : 13
}))

app.post('/settings', function(req, res) {
    res.json({
        status: 'success',
        openingTime : req.body.openingTime,
        closingTime : req.body.closingTime,
        openingDuration : req.body.openingDuration,
        closingDuration : req.body.closingDuration
    })
});

app.listen(port, () => console.log(`Fake server is listening on port ${port}!`))