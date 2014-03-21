print "Should not be printed\n";

sub no_args {
	print "7: no args\n";
}

sub args_no_return {
	if (scalar @_ == 0) {
		print "9: no args\n";
	}
	else {
		print "8: args: ";
		foreach(@_){
			print "$_ ";
		}
		print "\n";
	}
}

sub no_args_return {
	return "10: no args but return";
}

sub mult_return {
	return "11:", "mult", "return";
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