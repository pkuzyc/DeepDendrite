#!/usr/bin/perl -w

# Filter mod2c/mod2c_core output for comparison purposes.
# 1) rewrites #include lines
# 2) performs cpp preprocessing
# 3) tokenizes (naively) to one item per line
# 4) remove path string in "help ..."

# usage: filter_mod2c_output.pl [additional CPP options...] < mechanism.c
#        filter_mod2c_output.pl [additional CPP options...] -- mechanism.c

use File::Temp qw/tempfile/;

my @cpp_args=qw/-nostdinc -P/;
while (@ARGV) {
    my $a=shift @ARGV;
    next if $a eq '--';
    push @cpp_args,$a;
}

my ($fh,$fname) = tempfile(UNLINK=>1);
while (<>) {
    s/#\s*include\s*"([^"]*)"/INCLUDE("$1")/g;
    s/#\s*include\s*<([^>]*)>/INCLUDE_SYSTEM("$1")/g;

    $fh->print($_);
}
$fh->close();

open(CPP,"-|","cpp",@cpp_args,$fname) or die "failed to invoke cpp\n";
print map {s/"help .*"/"help XXX"/; "$_\n"} tokenize($_) while <CPP>;
close(CPP);

sub tokenize {
    my @ts=();
    while (/\s*(
        (?<q>['"]) (\\. | [^\\])*? (\g{q}|$) # single or double quoted string literal
        |
        [\w\d\$]+  # numeric literal or identifier
        |
        [(){},;\?:\[\]]  # delimiter or unambiguous single character operator
        |
        [^\s'"\w\d\$(){},;\?:\[\]]+ # sequence of punctuation otherwise not handled
       )\s*/gx)
    {
        push @ts,$1;
    }
    return @ts;
}
