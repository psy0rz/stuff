function user_event(domoticz, location, role)
    --domoticz.log(location)
    --domoticz.log(role)
    if role=="admin" then
       if location=="kantoor" then
           domoticz.openURL('http://192.168.13.55/control?cmd=cisa_unlockhard,60')
           -- domoticz.openURL('http://192.168.13.51/control?cmd=cisa_unlock,600')
           domoticz.devices('alarm kantoor').switchOff()
       end

       if location=="achter" then
           domoticz.openURL('http://192.168.13.51/control?cmd=cisa_unlockhard,600')
       end

       return
    end

    --domoticz.openURL('http://192.168.13.54/control?cmd=event,error')

end


return {
    active = true,
    on = {
        devices = {
            'RFID *'
        }
    },
    execute = function(domoticz, device)
        key="rfid_"..device.text
        if domoticz.variables(key) then
            value=domoticz.variables(key).value
            -- update the time
            domoticz.variables(key).set(value)
        else
            value="unnamed:new"
            domoticz.openURL('http://localhost/json.htm?type=command&param=saveuservariable&vname='..key..'&vtype=2&vvalue=unnamed:new')
        end

        domoticz.log('RFID: scanned '..key..' '..value)

        -- trigger event
        user_event(domoticz, device.name:gsub("^RFID ",""), value:gsub("^.*:",""))


    end

}
