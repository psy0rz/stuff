<?
	require_once("db.php");
	require_once("streams.php");
	require_once("streamsdb.php");
	
	DbSetVote($_REQUEST[movie_name], $_REQUEST[vote_value]);
	
	$html=GetRatingHtml($_REQUEST[movie_name],1);
	$html=str_replace("'","\'",$html);
	echo "span = document.getElementById('$_REQUEST[movie_name]');";
	echo "span.innerHTML = '$html';";


	