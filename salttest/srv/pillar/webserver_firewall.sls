#basic firewall config voor webservers

firewalld:
  enabled: True
  default_zone: public
  services:
    sshcustom:
      short: sshcustom
      description: SSH on port 3232 and 5252. Secure Shell (SSH) is a protocol for logging into and executing commands on remote machines. It provides secure encrypted communications. If you plan on accessing your machine remotely via SSH over a firewalled interface, enable this option. You need the openssh-server package installed for this option to be useful.
      ports:
        tcp: 
          - 3232
          - 5252
      modules: 
      destinations:
        ipv4: 
          - 224.0.0.251
          - 224.0.0.252
        ipv6: 
          - ff02::fb
          - ff02::fc
  zones:
    public:
      short: Public
      description: "For use in public areas. You do not trust the other computers on networks to not harm your computer. Only selected incoming connections are accepted."
      services:
        - http
        - https
        - ssh
        - dhcpv6-client
#      ports: 
#        - comment: For our dummy service
#          port: 1234
#          protocol: tcp
#      forward_ports:
#        - comment: forward 123 to other machine
#          portid: 123
#          protocol: tcp
#          to_port: 321
#          to_addr: 1.2.3.4
#      rich_rules:
#        - family: ipv4
#          source:
#            address: 192.168.1.0/24
#            invert: true
#          port:
#            portid: 123-321
#            protocol: udp
#          log:
#            prefix: local
#            level: notice
#            limit: 3/s
#          audit:
#            limit: 2/h
#          reject:
#            type: icmp-host-prohibited
