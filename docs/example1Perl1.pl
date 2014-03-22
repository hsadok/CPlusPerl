print "This will not be printed when called from C++\n";

sub showtime {
  print "Time: ", time, "\n";
}

sub sum {
  return $_[0] + $_[1];
}

sub distArg {
	print "DistArgs\n";
	foreach(@_){
		print "Arg: $_ \n";
	}
}

sub squareAll {
	print "square all!\n";
	my @ret;
	foreach(@_){
		print "$_ ";
		push (@ret, $_*$_);
	}
	print "\n";
	return @ret;
}

#showtime
#print soma(2,3), "\n";
#distArg(1,2,3);
