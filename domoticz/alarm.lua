-- Alarm device naming:
-- PIR sensors: "PIR (group) (name)"
-- Alarm on/off switch: "alarm (group) (name)"




indicator_ips={
  ["kantoor"] = {
    '192.168.13.52',
    '192.168.13.53',
  }
}

function string.starts(String,Start)
  return string.sub(String,1,string.len(Start))==Start
end

local function split(str,pat)
  local tbl={}
  str:gsub(pat,function(x) tbl[#tbl+1]=x end)
  return tbl
end


-- send event to espeasy indication units (lights/sirens etc)
local function indicate(domoticz, group, state)
  domoticz.log("Sending indication "..state.." to group "..group)
  for i, ip in ipairs(indicator_ips[group]) do
    domoticz.openURL("http://"..ip.."/control?cmd=event,"..state)
  end
end

local function switch_state(domoticz, group, state)
  if domoticz.data.states[group] ~= state then
    domoticz.log("Changing state of group "..group.." to: "..state)
    domoticz.data.states[group]=state
    indicate(domoticz, group, state)
  end


end


-- alarm states:
-- disarmed
-- warn_armed
-- armed
-- warn_alarm
-- alarm


return {
  active = true,
  on = {
    -- timer = {
    --     'every minute'
    -- },
    devices = {
      'PIR *',
      'alarm *'
    }
  },
  data = {
    states={ initial={} }
  },

  execute = function(domoticz, device, triggerInfo)

    -- -- timer event
    -- if triggerInfo.type == domoticz.EVENT_TYPE_TIMER then
    --
    --   -- no change in state needed
    --   if domoticz.data.state == domoticz.data.want_state then
    --     return
    --   end
    --
    --   if domoticz.data.state=="disarmed" then
    --
    --   end
    --
    --
    --   --            domoticz.openURL('http://192.168.13.52/control?cmd=pulse,4,1,25')
    --
    --   -- device event
    -- else
    parts=split(device.name, "[^ ]+")
    device_type=parts[1]
    device_group=parts[2]
    device_name=parts[3]

    alarm_state=domoticz.data.states[device_group]
    state_device=domoticz.devices("alarm "..device_group)


    -- motion detected
    if device_type=="PIR" then
      -- if device.lastUpdate.minutesAgo >= 15 then
      if alarm_state=='armed' then
        switch_state(domoticz, device_group, 'warn_alarm')
        state_device.switchOn().afterSec(10)
      elseif alarm_state=='warn_armed' then
        -- stop arming, try again after an hour
        state_device.switchOff()
        state_device.switchOn().afterSec(3600)
      end
      -- end
    end

    -- alarm switched on/off or change state (this is done by a time event via switchOn().afterSec())
    if device_type=="alarm" then

      if device.state == 'On' then
        -- alarm button is switched on (again). change state accordingly
        if alarm_state=='disarmed' then
          switch_state(domoticz, device_group, 'warn_armed')
          state_device.switchOn().afterSec(10)
        elseif alarm_state=='warn_armed' then
          switch_state(domoticz, device_group, 'armed')
        elseif alarm_state=='warn_alarm' then
          switch_state(domoticz, device_group, 'alarm')
          state_device.switchOn().afterSec(60)
        elseif alarm_state=='alarm' then
          switch_state(domoticz, device_group, 'armed')
        end
      else
        -- alarm button is switched off
        state_device.switchOff().silent()
        switch_state(domoticz, device_group, 'disarmed')
      end

    end
  end

  -- end

}
