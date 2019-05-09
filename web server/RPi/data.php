<!DOCTYPE html>
<html>
	<body>
		<h1>The enviroment data at present time is:</h1>
		<?php
			$opt=htmlspecialchars($_POST["data"]);
			echo " $opt";
			if($opt=="humidity")
			{
			var_dump(exec("head -1 current_data.txt"));
			}	
			elseif($opt=="temperature")
			{	
			var_dump(exec("tail -1 current_data.txt"));
			}
		//	elseif($opt=="log")
		//	{
		//	var_dump(exec(cat log_data.txt));
		//	}
		?>

	</body>
</html>