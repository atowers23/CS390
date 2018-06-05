#!/usr/bin/perl -w

# Andrew Towers
# Program 6
# This program parses a text file with fields separated by whitespace in the order: first name, last name,
# number of plate appearances, number of at bats, number of singles, number of doubles, number of triples,
# number of home runs, number of walks or hit by pitch.  It then calculates the batting average, slugging
# percentage, and on-base percentage and prints the formated results sorted alphabetically by lastname then
# firstname to the screen.  If there is not enough data fields or non numeric data in statistics field, an
# error is printed.  The program then prints a BATTING AVERAGE  report excluding invalid lines sorted in
# descending batting average order
use strict;
use warnings;
use diagnostics;

printf("---- BEGIN STATISTICS REPORT ----\n\n"); # print heading
printf("LASTNAME, FIRSTNAME     AVG   SLG   OBP\n");

my %player_data; # declare hash to store each player's data

PLAYER: while (<>) # for each line in the input file
{

  # store each field in the line seperated by whitespace as individual member of array
  my @player_entry = split(/\s+/);
  my $player_name = $player_entry[1] . ', ' . $player_entry[0]; # store the name in "last, first" format
  $player_data{$player_name} = {}; # declare an inner hash for player to be filled with batting data

  if ( $#player_entry < 8 ) # if the number of fields is less than 9
  {
    $player_data{$player_name}{'error'} = "Unable to compute result.  Not enough data. ***"; # store error type
    $player_data{$player_name}{'BA'} = 0; # initialize BA to prevent errors in BA sort
    next PLAYER; # stop execution and read next line
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

  my $plate_appearances = $player_entry[2]; # initialize variable for each field with corresponding value
  my $at_bats = $player_entry[3];
  my $singles = $player_entry[4];
  my $doubles = $player_entry[5];
  my $triples = $player_entry[6];
  my $homeruns = $player_entry[7];
  my $bb_plus_hbp= $player_entry[8];

  my $total_hits = $singles + $doubles + $triples + $homeruns; # compute total hits to be used in BA and OBP
  my $weighted_hits = $singles + ($doubles * 2) + ($triples * 3) + ($homeruns * 4); # compute weighted hits to be used in SLG

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
    if (exists($player_data{$player}{'error'})) # if player has invalid data
    {
      next; # don't print, go to next player
    }
    printf("   %-18s: %4.3f\n", $player, $player_data{$player}{'BA'}); # print player's name and BA
}

printf("\n---- END STATISTICS REPORT ----\n"); # Print closing line

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
    return $a_firstname cmp $b_firstname; #  then sort based on first name
  }
  else # else b's lastname comes before a's
  {
    return 1;
  }
}
# descending batting average sort function for %player_data hash
sub by_batting_average
{
  $player_data{$b}{'BA'} <=>$player_data{$a}{'BA'} # sort descending BA
}
