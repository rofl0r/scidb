<?php
	$filename = "../../persistent/" . $_POST['theme'];
	$values = @file_get_contents($filename, 1024);
	if (strlen($values) == 0)
		$values = "0,0,0,0,0";
	echo $values;
?>
