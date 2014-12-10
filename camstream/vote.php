<?
	require_once("db.php");
	require_once("streams2.php");
	
        $rating[votes]="26";
        $rating[mine]="3";
       	$rating[average]=$_REQUEST[vote_value];
	$rating[movie_name]=$_REQUEST[movie_name];
	$html=str_replace("'","\'",GetRatingHtml($rating));
	echo "span = document.getElementById('$_REQUEST[movie_name]');";
	echo "span.innerHTML = '$html';";


	