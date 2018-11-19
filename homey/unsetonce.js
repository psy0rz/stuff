//(C) edwin@datux.nl GPL

//if a variable doesnt exist or is true, make it false one time and return true
//otherwise return false

tag=args[0];
try
{
    let t = await Homey.flow.getToken({id:tag, 'uri':'homey:app:com.athom.homeyscript'});
    if (!t.value)
        return false;
    
}
catch(err)
{
    
}

await setTagValue(tag, {type: "boolean", title: tag}, false);
return true;
