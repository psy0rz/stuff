packages:
  pkg.installed:
    - pkgs:
      - nginx
#      - php-mbstring
#      - php-xml
#      - php-gd
#      - php-intl
#      - php-pdo
#      - php-mysql
#      - libxml2
#      - mariadb-server
#      - libicu
      - php-fpm




php-fpm:
  service.running:
    - enable: True
    - require:
      - pkg: packages

include:
   - firewalld
   - nginx.ng




