<?php
	error_reporting(E_ALL);
	ini_set('display_errors', 1);

	$basedir = "../../persistent/";
	$filename = $basedir . $_POST['theme'];
	$value = intval($_POST['value']);
	$vote = intval($_POST['vote']);

	if ($vote > 0 || $value > 0) {
		$lockdir = $basedir . "_lock_" . $_POST['theme'];

		for ($i = 0; $i < 5 && !($rc = mkdir($lockdir)); ++$i)
			usleep(50000);
		if (!$rc) {
			echo "timeout";
			return;
		}

		$values = @file_get_contents($filename, 1024);
		if (strlen($values) == 0)
			$values = "0,0,0,0,0";

		$tempfile = $filename . "_";
		$ft = fopen($tempfile, 'w');

		$arr = explode(",", $values);
		if ($value && $arr[$value - 1] > 0)
			$arr[$value - 1] -= 1;
		if ($vote)
			$arr[$vote - 1] += 1;
		$values = implode(",", $arr);

		fwrite($ft, $values);
		fclose($ft);

		if (!rename($tempfile, $filename)) {
			unlink($tempfile);
			$values = "failed";
		}

		rmdir($lockdir);
	} else {
		$values = file_get_contents($filename, 1024);
		if (strlen($values) == 0)
			$values = "0,0,0,0,0";
	}

	echo $values;
?>
