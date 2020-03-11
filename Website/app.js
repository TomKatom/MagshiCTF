const express = require("express")
const app = express();
const ejs = require("ejs")
let solvers = 0

app.set('view engine', 'ejs')

app.get('/', (req, res) => {
    res.render('index', {solvers: solvers})
})

app.get('/verify', (req, res) => {
    if(req.query.flag === "Magshi{m465h1m1m_7h3_n4710n4l_cyb3r_pr06r4m}"){
        solvers++
        res.sendStatus(200)
    }
    else {
        res.sendStatus(401)
    }
})

app.get('/download', (req, res) => {
    res.download('files/ctf.zip')
})

app.listen(4000)
