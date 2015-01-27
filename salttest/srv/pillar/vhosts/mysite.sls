#vhost config

nginx:
  ng:
    vhosts:
      managed:
        mysite: # relative pathname of the vhost file
          # may be True, False, or None where True is enabled, False, disabled, and None indicates no action
          #dir: /tmp # an alternate directory (not sites-available) where this vhost may be found
          #disabled_name: mysite.aint_on # an alternative disabled name to be use when not symlinking
          enabled: True
          
          # May be a list of config options or None, if None, no vhost file will be managed/templated
          # Take server directives as lists of dictionaries. If the dictionary value is another list of
          # dictionaries a block {} will be started with the dictionary key name
          config:
            - server:
              - server_name: localhost
              - listen: 
                - 80
                - default_server
              - index:
                - index.html
                - index.htm
              - location ~ .htm:
                - try_files:
                  - $uri
                  - $uri/ =404
                - test: something else
                
          # The above outputs:
          # server {
          #    server_name localhost;
          #    listen 80 default_server;
          #    index index.html index.htm;
          #    location ~ .htm {
          #        try_files $uri $uri/ =404;
          #        test something else;
          #    }
          # }         
