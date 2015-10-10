<?
require_once("streams.php");
require_once("streamsdb.php");
//Via deze file is het mogelijk om op een bepaalde positie te starten in de stream
//Alleen gebruiken indien nodig: inefficenter dan rechstreeks downloaden uiteraard.

//security
$movie_name=$_REQUEST[movie_name];
if (DbGetMovieId($movie_name,0)==0)
    die ("File not found!");

header( 'Content-type: video/x-ms-asf' );
header( 'Content-Transfer-Encoding: Binary' );
//           header( 'Content-length: '.filesize( $file ) );
//             header( 'Content-disposition: attachment;filename='.basename( $file ));



$start=(int)$_REQUEST[start];

//schrijf output naar tijdelijke file
$temp=tempnam("/tmp","streams");
system("vlc -I dummy --extraintf dummy --start-time $start '$movie_name' vlc:quit --sout '#std{access=file,dst=/dev/stdout,mux=asf}' </dev/null >$temp 2>/dev/null");
$fh=fopen($temp,"r");

//delete hem alvast, zodat hij automatisch verdwijnt als apache er mee klaar is!
unlink($temp);

//stuur naar user
fpassthru($fh);
fclose($fh);
