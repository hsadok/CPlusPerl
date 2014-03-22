
sub showtime {
  print "Time 2: ", time, "\n";
}

sub sum {
  print "(Sum 2) $_[0] + $_[1]\n";
  return $_[0] + $_[1];
}

sub distArg {
	print "DistArgs 2\n";
	foreach(@_){
		print "Arg: $_ \n";
	}
}

sub squareAll {
	print "square all! (2)\n";
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
