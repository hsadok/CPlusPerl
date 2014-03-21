print "Should not be printed\n";

sub no_args {
	print "1: no args\n";
}

sub args_no_return {
	if (scalar @_ == 0) {
		print "3: no args\n";
	}
	else {
		print "2: args: ";
		foreach(@_){
			print "$_ ";
		}
		print "\n";
	}
}

sub no_args_return {
	return "4: no args but return";
}

sub mult_return {
	return "5:", "mult", "return";
}

sub pass_func_ret {
	return "func", "ret", $@[0];
}

sub disp_arg {
	foreach(@_){
		print "$_ ";
	}
	print "\n";
}