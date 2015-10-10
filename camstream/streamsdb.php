<?
    require_once("db.php");
    require_once("streams.php");    
    
    function DbGetUserByIp($ip)
    {
    	$user=db_queryfetchrow("
	    SELECT *
	    FROM users
	    WHERE user_ip='$ip'
	");
	return ($user);    
    }
    

    function DbGetCurrentUser()
    {
	global $HTTP_SERVER_VARS;
    
	//bestoan wie al??
	$user=DbGetUserByIp($HTTP_SERVER_VARS[REMOTE_ADDR]);
	
	//bestaat nie!
	if (!$user)
	{
	    //maak noaie
	    $user[user_ip]=$HTTP_SERVER_VARS[REMOTE_ADDR];
	    db_SetRow("users",0,$user);
	    
	    $user=DbGetUserByIp($HTTP_SERVER_VARS[REMOTE_ADDR]);
	}
	
	return ($user);
    }
    

    function DbGetMovieId($movie_name,$auto_create=1)
    {
    	$movie=db_queryfetchrow("
	    SELECT *
	    FROM movies
	    WHERE movie_name='$movie_name'
	");

	//bestoat noait?	
	if (!$movie)
	{
	    if (!$auto_create)
	    {
		return (0);
	    }
	    $movie[movie_name]=$movie_name;
	    db_SetRow("movies",0,$movie);
	    return(db_InsertId());
	}
	
	return($movie[movie_id]);	
    }

    /** Bepaald rating van meegeven movie
     * Gebruikt bayesian rating 
     */
    global $min_votes;
    $min_votes=5;
    function DbGetRating($movie_name)
    {
	global $min_votes;
	
	$movie_id=DbGetMovieId($movie_name);
	
	//gemiddelde en aantal votes van DEZE movie
	$movie_avg=db_QueryFetchRow("
	    SELECT 
		AVG(vote_value) as avg,
		COUNT(vote_value) as count
	    FROM votes
	    WHERE movie_id='$movie_id'
	");

	//gemiddelde van ALLE movies
	$total_avg=db_QueryFetchRow("
	    SELECT 
		AVG(vote_value) as avg
	    FROM votes
	");
	
	//vote die de huidige user gegeven heeft.
	$user=DbGetCurrentUser();
	$vote=db_QueryFetchRow("
	    SELECT * 
	    FROM votes
	    WHERE user_id='$user[user_id]'
	    AND	movie_id='$movie_id'
	");
	
	
	//bayesian gemiddelde
	//br = ( (avg_num_votes * avg_rating) + (this_num_votes * this_rating) ) / (avg_num_votes + this_num_votes)
	//min_votes is de 'magic factor' om de boel te tunen
	$avg_num_votes=5;
	$rating['avg']= ( ($min_votes * $total_avg['avg']) + ($movie_avg['count'] * $movie_avg['avg']) ) / ($min_votes + $movie_avg['count']);
	$rating['count']=$movie_avg['count'];
	$rating['vote_value']=$vote['vote_value'];
	
	return($rating);
    }
    
    
    function DbGetTop($limit)
    {
	global $min_votes;

	//gemiddelde van ALLE movies bijelkaar
	$total_avg=db_QueryFetchRow("
	    SELECT 
		AVG(vote_value) as avg
	    FROM votes
	");
	
	// movies met hoogstge bayesian ratings
	$tops=db_QueryFetch("
	    SELECT
		movies.*,
		(( ($min_votes * $total_avg[avg]) + COUNT(vote_value) * AVG(vote_value) ) / ($min_votes + COUNT(vote_value))) as avg
	    FROM votes,movies
	    WHERE movies.movie_id=votes.movie_id
	    GROUP by movie_id
	    ORDER by avg DESC
	    LIMIT $limit
	");

	// rearrange to this form:
	// $ret[movie_name]=array($thumbs);
	foreach ($tops as $top)
	{
	    $ret[$top[movie_name]]=GetThumbs($top[movie_name]);
	}
	return($ret);
    }
    
    function DbSetVote($movie_name, $vote_value)
    {
	if ($vote_value<0)
	    return ;
	
	if ($vote_value>5)
	    return ;
    
	// welke movie en user id?
	$movie_id=DbGetMovieId($movie_name);
	$user=DbGetCurrentUser();
	$vote[movie_id]=$movie_id;
	$vote[user_id]=$user[user_id];
	$vote[vote_value]=$vote_value;

	// bestaande vote weghalen
	db_DoQuery("
	    DELETE 
	    FROM votes
	    WHERE user_id='$vote[user_id]'
	    AND	movie_id='$vote[movie_id]'
	");
	
	// noaie toevoegn
	db_SetRow("votes",0,$vote);
	
    }    


    //verwijder overbodig files en zooi
    function DbCleanup()
    {
	//ALLE movies van ouder dan 5 dagen
	$movies=getmovies("biercam",0,time()-(5*3600*24));
	
	foreach ($movies as $movie_name=>$thumbs)
	{
	    //heeft ie al een entry in de database?
	    $id=DbGetMovieId($movie_name,0);
	    if ($id)
	    {
		//ja hij heeft al stemmen
		//heeft ie stemmen hoger dan 1?
		$votes=db_QueryFetchRow("
		    SELECT *
		    FROM votes
		    WHERE movie_id=$id
		    AND vote_value>1
		    LIMIT 1
		");
		if ($votes)
		    continue;
	    }
	    //hij kan dus weg:
	    echo "$movie_name kan weg!\n";
//	    echo " <a href='view.php?movie_name=$movie_name'>zien</a><br>";

	    //verwijder thumbs, movies, dbentry en votes
	    unlink($movie_name);
	    foreach ($thumbs as $thumb)
		unlink($thumb);
	    Db_DoQuery("
		DELETE FROM votes
		WHERE movie_id=$id
	    ");
	    Db_DoQuery("
		DELETE FROM movies
		WHERE movie_id=$id
	    ");
	    
	}
    }
    
    
    