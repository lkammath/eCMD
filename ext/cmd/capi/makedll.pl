#!/usr/bin/perl
# File makedll.pl created by Joshua Wills at 12:45:07 on Fri Sep 19 2003. 

#functions to ignore in parsing ecmdClientCapi.H
my @ignores = qw( ecmdLoadDll ecmdUnloadDll ecmdCommandArgs);
my $ignore_re = join '|', @ignores;

my $printout;
my @enumtable;

open IN, "ecmdClientCapi.H" or die "Could not find ecmdClientCapi.H: $!\n";

open OUT, ">ecmdDllCapi.H" or die $!;
print OUT "/* The following has been auto-generated by makedll.pl */\n";

print OUT "#ifndef ecmdDllCapi_H\n";
print OUT "#define ecmdDllCapi_H\n\n";

print OUT "#include <inttypes.h>\n";
print OUT "#include <ecmdStructs.H>\n";
print OUT "#include <ecmdReturnCodes.H>\n";
print OUT "#include <ecmdDataBuffer.H>\n\n\n";

print OUT "extern \"C\" {\n\n";

print OUT "int dllLoadDll (const char * version);\n\n";
print OUT "int dllInitDll ();\n\n";
print OUT "int dllUnloadDll ();\n\n";


#parse file spec'd by $ARGV[0]
while (<IN>) {

    if (/^int/) {

	next if (/$ignore_re/);

	chomp; chop;  
	my ($func, $args) = split /\(|\)/ , $_;

	my ($type, $funcname) = split /\s+/, $func;
	my @argnames = split /,/ , $args;

        #remove the default initializations
        foreach my $i (0..$#argnames) {
            if ($argnames[$i] =~ /=/) {
              $argnames[$i] =~ s/=.*//;
            }
        }

        $" = ",";
        $printout .= "$type $funcname(@argnames) {\n\n";

	$printout .= "  $type rc;\n\n";
	$printout .= "#ifdef ECMD_STATIC_FUNCTIONS\n\n";

	$printout .= "  rc = ";

        my $enumname;

        if ($funcname =~ /ecmd/) {

           $funcname =~ s/ecmd//;

           $enumname = "ECMD_".uc($funcname);

           $funcname = "dll".$funcname;
        }
        else {

           $enumname = "ECMD_".uc($funcname);
           $funcname = "dll".ucfirst($funcname);
        }

        print OUT "$type $funcname(@argnames); \n\n";
	$" = " ";
       
	  
	$printout .= $funcname . "(";

	my $argstring;
	my $typestring;
	foreach my $curarg (@argnames) {

	    my @argsplit = split /\s+/, $curarg;

	    my @typeargs = @argsplit[0..$#argsplit-1];
	    $typestring .= "@typeargs" . ", ";

	    $argstring .= $argsplit[-1] . ", ";
	}

	chop ($typestring, $argstring);
	chop ($typestring, $argstring);

	$printout .= $argstring . ");\n\n";
	    
	$printout .= "#else\n\n";

	$printout .= "  if (dlHandle == NULL) {\n";
	$printout .= "     return ECMD_DLL_UNINITIALIZED;\n";
        $printout .= "  }\n\n";
	$printout .= "  if (DllFnTable[$enumname] == NULL) {\n";
	$printout .= "     DllFnTable[$enumname] = (void*)dlsym(dlHandle, \"$funcname\");\n";
	$printout .= "  }\n\n";

	$printout .= "  $type (*Function)($typestring) = \n";
	$printout .= "      ($type(*)($typestring))DllFnTable[$enumname];\n\n";

	$printout .= "  rc = (*Function)($argstring);\n\n";
	
	$printout .= "#endif\n\n";

	$printout .= "  return rc;\n\n";

	$printout .= "}\n\n";

	push @enumtable, $enumname;
    }

}
close IN;

print OUT "} //extern C\n\n";
print OUT "#endif\n";
print OUT "/* The previous has been auto-generated by makedll.pl */\n";

close OUT;  #ecmdDllCapi.H

open OUT, ">ecmdClientCapiFunc.C" or die $!;

print OUT "/* The following has been auto-generated by makedll.pl */\n";
print OUT "\n#include <dlfcn.h>\n\n";
print OUT "#include <ecmdClientCapi.H>\n";
print OUT "#include <ecmdDllCapi.H>\n\n\n";
print OUT "#ifndef ECMD_STATIC_FUNCTIONS\n";


push @enumtable, "ECMD_NUMFUNCTIONS";
$" = ",\n";
print OUT "typedef enum {\n@enumtable\n} ecmdFunctionIndex_t;\n\n";
$" = " ";

print OUT "void * dlHandle = NULL;\n";
print OUT "void * DllFnTable[ECMD_NUMFUNCTIONS];\n";

print OUT "#endif\n\n\n";

print OUT $printout;

print OUT "/* The previous has been auto-generated by makedll.pl */\n";

close OUT;  #ecmdClientCapiFunc.C
