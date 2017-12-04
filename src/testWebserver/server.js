const express = require('express')
const app = express()

app.get('/up', (req, res) => res.send('up'))
app.get('/down', (req, res) => res.send('down'))
app.get('/open', (req, res) => res.send('open'))
app.get('/close', (req, res) => res.send('closed'))
app.get('/stop', (req, res) => res.send('stopped'))

app.use('/', express.static('./'))

app.listen(3000, () => console.log('Example app listening on port 3000!'))