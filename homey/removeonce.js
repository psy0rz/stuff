//(C) edwin@datux.nl GPL

//if a variable exists, remove it and return true
//otherwise return false

tag=args[0];
try
{
    let t = await Homey.flow.getToken({id:tag, 'uri':'homey:app:com.athom.homeyscript'});
    await setTagValue(tag, {type: "boolean", title: tag}, undefined);
    return true;    
}
catch(err)
{
    //already removed
    return false;    
}
