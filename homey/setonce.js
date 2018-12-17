//(C) edwin@datux.nl GPL
// Tested with Homey v2.0-rc4

//if a variabele doesnt exist or is false, set it to true one time and return true.
//otherwise return false

tag=args[0];
try
{
    let t=await Homey.flowToken.getFlowToken({id:tag, 'uri':'homey:app:com.athom.homeyscript'});
    if (t.value)
        return false;

}
catch(err)
{

}

await setTagValue(tag, {type: "boolean", title: tag}, true);
return true;
