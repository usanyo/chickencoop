
const char index_html[] = R"=====(
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
                width: 16pc;
                height: 16pc;
                margin: 20px;
                background-size: 16pc, 16pc;
                background-color: white;
                border-width: 0px;
            }
            .open {
                background-image: url("https://www.iconexperience.com/_img/o_collection_png/green_dark_grey/256x256/plain/lock_open.png");
            }

            .close {
                background-image: url("https://www.iconexperience.com/_img/o_collection_png/green_dark_grey/256x256/plain/lock.png");
            }

            .up {
                background-image: url("http://icons.iconarchive.com/icons/graphicloads/100-flat-2/256/arrow-up-icon.png");
            }

            .down {
                background-image: url("http://icons.iconarchive.com/icons/graphicloads/100-flat-2/256/arrow-down-icon.png");
            }

            .stop {
                background-image: url("https://images.onlinelabels.com/images/clip-art/Leomarc/Leomarc_stop_sign.png");
            }
        </style>
    </head>
    <body>
        <p>
            <br>
            <button class="open" onclick="exec('open')"></button>
            <button class="up" onclick="exec('up')"></button>
            <br>
            <button class="close" onclick="exec('close')"></button>
            <button class="down" onclick="exec('down')"></button>
            <br>
            <button class="stop" onclick="exec('stop')"></button>
        </p>
    </body>
</html>
)=====";
