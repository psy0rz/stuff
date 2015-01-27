#vhost config
nginx:
  ng:
    vhosts:
      managed:

        ########################
        test.local.conf: 
          enabled: True
          
          config:
            server:
              root: /var/www/test.local
              server_name: test.local
              listen: 
                - 80
                - default_server
              index:
                - index.html
                - index.htm
              location ~ .htm:
                - try_files:
                  - $uri
                  - $uri/ =404
                
