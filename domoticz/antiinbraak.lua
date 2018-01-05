

dimmers={
       'keuken tafel',
       'woonkamer tv lamp',
       'woonkamer tafeltje',
       'keuken plafond'
}



return {
    active = true,
    on = {
        timer = {
            'every 5 minutes'
        },
        devices = {
            'anti inbraak',
        }
    },
 
    execute = function(domoticz, device, triggerInfo)
        
        -- timer event
        if triggerInfo.type == domoticz.EVENT_TYPE_TIMER then


--            domoticz.openURL('http://192.168.13.52/control?cmd=pulse,4,1,25')[<32;1;31M[<32;1;31M
            if domoticz.devices('anti inbraak').state=='On' then
              dimmer=dimmers[math.random(4)]
              if math.random(2)==1 then
                  level=math.random(100)
                  domoticz.log(dimmer.." level "..level)
                  domoticz.devices(dimmer).dimTo(level)
              else
                  domoticz.log(dimmer.." off")
                  domoticz.devices(dimmer).switchOff()
              end

            end

        -- device event
        else
            if device.name=="anti inbraak" then
               if device.state == 'Off' then
                 for i, dimmer in ipairs(dimmers) do
                        domoticz.devices(dimmer).switchOff()
                   end
               end
--               if device.state == 'On' then
--                 for i, dimmer in ipairs(dimmers) do
--                        domoticz.devices(dimmer).switchOn()
--                   end
--               end
            end
        end
    end
}
