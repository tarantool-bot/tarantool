const char box_http_lua[] =
"-- box.http\n"
"\n"
"(function(box)\n"
"    local function errorf(fmt, ...)\n"
"        error(string.format(fmt, ...))\n"
"    end\n"
"\n"
"    local function retcode(code, reason)\n"
"        return nil, {\n"
"            Status  = code,\n"
"            Reason  = reason\n"
"        }\n"
"    end\n"
"\n"
"    local function connect(host, port)\n"
"        local s = box.socket.tcp()\n"
"        if s == nil then\n"
"            return nil, \"Can't create socket\"\n"
"        end\n"
"        local res = { s:connect(host, port) }\n"
"        if res[1] == nil then\n"
"            return nil, res[4]\n"
"        end\n"
"        return s\n"
"    end\n"
"\n"
"\n"
"\n"
"    box.http = {\n"
"        request = function(method, url, hdrs, body)\n"
"            if hdrs == nil then\n"
"                hdrs = {}\n"
"            end\n"
"            method = string.upper(method)\n"
"\n"
"            if method ~= 'GET' and method ~= 'POST' then\n"
"                return retcode(599, \"Unknown request method: \" .. method)\n"
"            end\n"
"\n"
"            local scheme, host, port, path, query = box.http.split_url( url )\n"
"\n"
"            if scheme ~= 'http' then\n"
"                return retcode(599, \"Unknown scheme: \" .. scheme)\n"
"            end\n"
"\n"
"            if string.len(host) < 1 then\n"
"                return retcode(595, \"Can't route host\")\n"
"            end\n"
"\n"
"            if port == nil then\n"
"                port = 80\n"
"            elseif string.match(port, '^%d+$') ~= nil then\n"
"                    port = tonumber(port)\n"
"            else\n"
"                return retcode(599, \"Wrong port number: \" .. port)\n"
"            end\n"
"\n"
"            local s, err = connect(host, port)\n"
"            if s == nil then\n"
"                return retcode(595, err)\n"
"            end\n"
"\n"
"\n"
"            if body == nil then\n"
"                body = ''\n"
"            end\n"
"\n"
"            local hdr = ''\n"
"            for i, v in pairs(hdrs) do\n"
"                if i ~= 'Content-Length' then\n"
"                    hdr = hdr .. string.format(\"%s: %s\\r\\n\", i, v)\n"
"                end\n"
"            end\n"
"\n"
"            if string.len(body) > 0 then\n"
"                hdr = hdr .. \"Content-Length: 0\\r\\n\"\n"
"            end\n"
"\n"
"            hdr = hdr .. \"User-Agent: Tarantool box.http agent\\r\\n\"\n"
"\n"
"            local pquery = ''\n"
"\n"
"            if string.len(query) > 0 then\n"
"                pquery = '?' .. query\n"
"            end\n"
"\n"
"\n"
"            local req = string.format(\n"
"                \"%s %s%s HTTP/1.1\\r\\n\" ..\n"
"                \"Host: %s\\r\\n\" ..\n"
"                \"%s\\r\\n\" ..\n"
"                \"%s\",\n"
"\n"
"                    method,\n"
"                    path,\n"
"                    pquery,\n"
"                    host,\n"
"                    hdr,\n"
"                    body\n"
"            )\n"
"\n"
"\n"
"            local res = { s:send(req) }\n"
"\n"
"            if #res > 1 then\n"
"                return retcode(595, res[4])\n"
"            end\n"
"            if res[1] ~= string.len(req) then\n"
"                return retcode(595, \"Can't send request\")\n"
"            end\n"
"                \n"
"\n"
"            return req, res\n"
"\n"
"\n"
"        end\n"
"    }\n"
"end)(box)\n"
"\n"
""
;