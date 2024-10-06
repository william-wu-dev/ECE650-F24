# Web Application Example

* Web server is implemented in Python in `jobrequest-web_app.py`
* Request processing is implemented in C++ in `processjobs.c`

## Running

Start the web server

```
> python3 jobrequest-web_app.py
```

Visit http://localhost:8080 using multiple browsers. Each visit generates a job
request at `/tmp.jobrequests`

Start job processing
```
> ./processjobs
```

Watch jobs being processed and processes fighting for resources and failing to lock files

Based on original example from Carlos Moreno. Adapted by Arie Gurfinkel.
