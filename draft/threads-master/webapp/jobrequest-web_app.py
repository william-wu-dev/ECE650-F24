import time
import http.server
import os
import os.path
import socket

# adapted from: https://wiki.python.org/moin/BaseHttpServer
try:
    hostName = socket.gethostname()
    IPAddr = socket.gethostbyname(hostName)
    HOST_NAME = IPAddr
except Exception:
    HOST_NAME = ""

#HOST_NAME = ""
PORT_NUMBER = 8080

class MyHandler(http.server.BaseHTTPRequestHandler):
    def do_HEAD(s):
        s.send_response(200)
        s.send_header(b"Content-type", "text/html")
        s.end_headers()

    def do_GET(s):
        """Respond to a GET request."""

        ip, port = s.client_address
        user = s.headers['User-Agent'].strip().split(' ')[0]

        status = 'Request queued for processing'
        try:
            tmp_dir = "/tmp/.jobrequests"
            os.makedirs(tmp_dir, exist_ok=True)

            fname = '{0:x}-{1}.txt'.format(time.time_ns(), os.getpid())
            tname = os.path.join(tmp_dir, 'tmp-' + fname)

            with open(tname, 'w') as f:
                ltime = time.localtime()
                date_str = time.strftime('%y-%m-%d', ltime)
                time_str = time.strftime('%H-%M-%S', ltime)
                msg = '{date} {time}\n{user} {ip}'.format(date=date_str,
                                                          time=time_str,
                                                          user=user,
                                                          ip=ip)
                f.write(msg)

            os.rename(tname, os.path.join(tmp_dir, fname))
        except Exception as e:
            status = 'Failed to process request: ' + str(e)
            print(e)

        # send response
        s.send_response(200)
        s.send_header(b"Content-type", b"text/html")
        s.end_headers()
        s.wfile.write(b"<html><head><title>Title goes here.</title></head>")
        s.wfile.write(b"<body><p>This is a test.</p>")
        # If someone went to "http://something.somewhere.net/foo/bar/",
        # then s.path equals "/foo/bar/".
        s.wfile.write(("<p>You accessed path: %s</p>" % s.path).encode())

        s.wfile.write(("<p>Your IP: {0}</p>".format(ip)).encode())
        s.wfile.write(("<p>Your browser is {0}</p>".format(user)).encode())
        s.wfile.write('<p>{0}</p>'.format(status).encode())
        s.wfile.write(b"</body></html>")


if __name__ == "__main__":
    server_class = http.server.HTTPServer
    httpd = server_class((HOST_NAME, PORT_NUMBER), MyHandler)
    print(time.asctime(), "Server Starts - %s:%s" % (HOST_NAME, PORT_NUMBER))
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass
    httpd.server_close()
    print(time.asctime(), "Server Stops - %s:%s" % (HOST_NAME, PORT_NUMBER))
