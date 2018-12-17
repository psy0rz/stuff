//(C) edwin@datux.nl GPL
// Tested with Homey v2.0-rc4

//if a variable exists, remove it and return true
//otherwise return false

tag=args[0];
try
{
    let t=await Homey.flowToken.getFlowToken({id:tag, 'uri':'homey:app:com.athom.homeyscript'});
    await setTagValue(tag, {type: "boolean", title: tag}, undefined);
    return true;
}
catch(err)
{
    //already removed
    return false;
}
