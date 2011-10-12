#!/usr/bin/perl
sub le_arquivo
{
	my $arq_name = shift;

	open(FILE, "<$arq_name")
		|| die "Can't open $arq_name.\n";

	my $content;

	while(<FILE>)
	{
		$content.= $_;
	}
	close(FILE);
	return $content;
}

sub tirarJavaScript1
{
	#my $content = `cat $_[0]`;
	#my $content = le_arquivo($_[0]);
    my $content = shift;

	$content =~ /(<title>.+?<\/title>)/;
	my $titulo = $1;
	
	$content =~ s/<meta.+?>//gsi;
	$content =~ s/<head.+?<body/<body/gis;
	$content =~ s/<script.+?<\/script>/ /gis;
	$content =~ s/<style.+?<\/style>/ /gis;
	$content =~ s/<iframe.+?<\/iframe>/ /gis;
	$content =~ s/<!.+?>/ /gis;
	$content =~ s/&\#8226;/ /gis;
	
	$content =~ s/<html(.*?)>\s*?<body/<html$1><head>$titulo<\/head><body/gis;
	$content =~ s/<TEXTAREA(.+?)\/>/<TEXTAREA$1><\/TEXTAREA>/gsi;
	
	
	return $content;

}

sub tirarJavaScript2
{
	#my $content = `cat $_[0]`;
	#my $content = le_arquivo($_[0]);
        my $content = shift;

	$content =~ s/<TEXTAREA(.+?)\/>/<TEXTAREA$1><\/TEXTAREA>/gsi;
	
	$content =~ s/^\s+/ /gis;
	$content =~ s/\s+$/\n/gis;
	$content =~ s/\s+/ /gis;
	
	$content =~ s/\s+<(.+?)>/<$1>/gis;
	$content =~ s/<(.+?)>\s+/<$1>/gis;
	
	return $content;

}


unless(scalar @ARGV == 1)
{
    die "usage: $0 <arquivo html>\n";
}

my $content = le_arquivo(shift);
my $limpo1 = tirarJavaScript1($content);
my $limpo2 = tirarJavaScript2($limpo1);

print $limpo2;


