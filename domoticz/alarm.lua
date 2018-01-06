-- Alarm device naming:
-- PIR sensors: "PIR (group) (name)"
-- Alarm on/off state and switch: "alarm (group)"
-- Alarm push button (to enable/disable alarm): "alarmpush (group)"
-- Automaticly enable alarm after a certain idle time: "alarmauto (group)"

-- internal alarm states:
--  disarmed
--  warn_armed
--  armed
--  warn_alarm
--  alarm

-- These are also send to ESPEasy devices as event. You should make ESPEasy rules to handle these events and actually control lamps and sirens.


indicator_ips={
  ["kantoor"] = {
    '192.168.13.52',
    '192.168.13.53',
  }
}

auto_arm_time=2*3600
-- auto_arm_time=10

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




return {
  active = true,
  on = {
    devices = {
      'PIR *',
      'alarm *',
      'alarmpush *',
      'alarmauto *'
    }
  },
  data = {
    states={ initial={} }
  },

  execute = function(domoticz, device, triggerInfo)


    parts=split(device.name, "[^ ]+")
    device_type=parts[1]
    device_group=parts[2]
    device_name=parts[3]

    alarm_state=domoticz.data.states[device_group]
    state_device=domoticz.devices("alarm "..device_group)


    local function check_auto_arm()
      if domoticz.devices("alarmauto "..device_group).state == 'On' then
        state_device.switchOn().afterSec(auto_arm_time)
      end
    end


    if device_type=="PIR" then
      -- motion detected

      if alarm_state=='armed' then
        switch_state(domoticz, device_group, 'warn_alarm')
        state_device.switchOn().afterSec(10)
      elseif alarm_state=='warn_armed' then
        -- stop arming
        state_device.switchOff()
        check_auto_arm()
      elseif alarm_state=='disarmed' then
        -- auto arm after there has been no movement for a long time
        check_auto_arm()
      end


    elseif device_type=="alarm" then
      -- alarm switched on/off or change state (this is done by a time event via switchOn().afterSec())

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
        -- alarm button is switched off. reset any timers
        state_device.switchOff().silent()
        switch_state(domoticz, device_group, 'disarmed')
        check_auto_arm()
      end

    elseif device_type=="alarmpush" then
      -- alarm pushbutton is pressed to enable/disable alarm.
      if device.state == 'On' then
        -- enable alarm
        if alarm_state=='disarmed' then
          state_device.switchOn()
        else
          -- disable alarm
          state_device.switchOff()
        end
      end


    elseif device_type=="alarmauto" then
      -- alarm auto arm mode has changed
      if device.state == 'On' then
        check_auto_arm()
      else
        -- reset any auto arm timers
        if state_device.state=='Off' then
          state_device.switchOff().silent()
        end
      end
    end
  end


}
