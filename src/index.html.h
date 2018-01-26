#include <html.h>

HTML(index_html) = R"=====(
<html>
    <head>
        <script>
            function exec(command) {
                httpGetAsync(command, (res) => console.log(res))
            }

            function httpGetAsync(url, callback) {
                var req = new XMLHttpRequest();
                req.onreadystatechange = () => { 
                    if (req.readyState == 4 && req.status == 200)
                        callback(req.responseText);
                }
                req.open('GET', url, true);
                req.send(null);
            }
        </script>
        <style>
            button {
                height: 8pc;
                width: 16pc;
                font-size: 3.5pc;
                margin: 20px;
            }
        </style>
    </head>
    <body>
        <p>
            <br>
            <button onclick="exec('open')">OPEN</button>
            <button onclick="exec('up')">UP</button>
            <br>
            <button onclick="exec('close')">CLOSE</button>
            <button onclick="exec('down')">DOWN</button>
            <br>
            <button onclick="exec('stop')">STOP</button>
        </p>
    </body>
</html>
)=====";
