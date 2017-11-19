#!/usr/bin/perl -w
use strict;
use warnings;
use diagnostics;

printf("---- BEGIN STATISTICS REPORT ----\n\n"); # print heading
printf("LASTNAME, FIRSTNAME     AVG   SLG   OBP\n");

my %player_data; # delcare hash to store each player's data

PLAYER: while (<>) # for each line in the input file
{

  my @player_entry = split(/\s+/); # store each field seperated by whitespace in the line as individual member of array
  my $player_name = $player_entry[1] . ', ' . $player_entry[0]; # store the name in "last, first" format
  $player_data{$player_name} = {}; # declare a hash to be filled with batting data

  if ( $#player_entry < 8 ) # if the number of fields is less than 9
  {
    $player_data{$player_name}{'error'} = "Unable to compute result.  Not enough data. ***"; # store error type
    $player_data{$player_name}{'BA'} = 0; # initialize BA to prevent errors in BA sort
    next PLAYER; # stop execution and go to PLAYER label at while (<>) statement
  }
  for (my $index = 2; $index < 9; $index++) # For each numeric field
  {
    if ( $player_entry[$index] !~ /^\d+$/) # If the field is not entirely digits
    {
      $player_data{$player_name}{'error'} = "Unable to compute result.  Invalid data ***";
      $player_data{$player_name}{'BA'} = 0;
      next PLAYER;
    }
  }

  my $plate_appearances = $player_entry[2]; # initialize variable for each field and store corresponding value
  my $at_bats = $player_entry[3];
  my $singles = $player_entry[4];
  my $doubles = $player_entry[5];
  my $triples = $player_entry[6];
  my $homeruns = $player_entry[7];
  my $bb_plus_hbp= $player_entry[8];

  my $total_hits = $singles + $doubles + $triples + $homeruns; # compute total hits to be used in BA and OBP
  my $weighted_hits = $singles + $doubles * 2 + $triples * 3 + $homeruns * 4; # compute weighted hits to be used in SLG

  $player_data{$player_name}{'BA'} = $total_hits / $at_bats; # calculate output values and store in player's hash
  $player_data{$player_name}{'SLG'} = $weighted_hits / $at_bats;
  $player_data{$player_name}{'OBP'} = ($total_hits + $bb_plus_hbp) / $plate_appearances;

}

foreach my $player (sort by_lastname keys %player_data) # sort each player in hash by "last, first"
{
  printf("   %-18s: ", $player); # print player's name
  if (exists($player_data{$player}{'error'})) # if player had invalid data
  {
    print "$player_data{$player}{'error'}\n"; # print error statement
    next; #do not print statistics, go to next player
  }
  # print formatted batting data
  printf("%4.3f %4.3f %4.3f\n", $player_data{$player}{'BA'}, $player_data{$player}{'SLG'}, $player_data{$player}{'OBP'});
}
printf("\n\n---- BATTING AVERAGE REPORT ----\n\n"); # print batting average header
printf("LASTNAME, FIRSTNAME     AVG\n");
foreach my $player (sort by_batting_average keys %player_data) # sort each player by batting average
{
    if (exists($player_data{$player}{'error'})) # if player had invalid data
    {
      next; # go to next player
    }
    printf("   %-18s: %4.3f\n", $player, $player_data{$player}{'BA'}); # print player's name and BA
}

printf("\n---- END STATISTICS REPORT ----\n");

# "last, first" sort function
sub by_lastname
{
  my $a_lastname; # initialize variables
  my $a_firstname;
  my $b_lastname;
  my $b_firstname;
  ($a_lastname, $a_firstname) = split(', ', $a); # seperate entries on comma and store into variables
  ($b_lastname, $b_firstname) = split(', ', $b);
  if (($a_lastname cmp $b_lastname) < 0) # if a's last name comes before b's last name in alphabetical order
  {
    return -1;
  }
  elsif (($a_lastname cmp $b_lastname) == 0) # if a and b have same last name
  {
    return $a_firstname cmp $b_firstname; # sort based on first name
  }
  else # else b's name comes before a's
  {
    return 1;
  }
}
sub by_batting_average
{
  $player_data{$b}{'BA'} <=>$player_data{$a}{'BA'} # sort descending BA
}
