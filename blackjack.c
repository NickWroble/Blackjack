#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<string.h>
#include<ctype.h>

char * getCardName(int cardVal){
  //returns the value of the card to be used in the terminal
  int val = (cardVal / 4) + 1;
  if(val == 1)return "ace";
  if(val == 2)return "two";
  if(val == 3)return "three";
  if(val == 4)return "four";
  if(val == 5)return "five";
  if(val == 6)return "six";
  if(val == 7)return "seven";
  if(val == 8)return "eight";
  if(val == 9)return "nine";
  if(val == 10)return "ten";
  if(val == 11)return "jack";
  if(val == 12)return "queen";
  else return "king";
}

int getNumericaVal(int cardVal){
  //returns the value of the card to be used to generate the sum
  int val = (cardVal / 4) + 1;
  if(val == 1)return 11;
  if(val == 2)return 2;
  if(val == 3)return 3;
  if(val == 4)return 4;
  if(val == 5)return 5;
  if(val == 6)return 6;
  if(val == 7)return 7;
  if(val == 8)return 8;
  if(val == 9)return 9;
  else return 10;
}

char * getCardSuit(int cardVal){
  //returns the suit of the card to be used in the terminal
  int suitVal = cardVal % 4;
  if(suitVal == 0) return "hearts";
  if(suitVal == 1) return "diamonds";
  if(suitVal == 2) return "clubs";
  else return "spades";
}

int getCardVal(int * tempCardList){
  //gets a random card from the deck, puts -1 into a used card postion to
  //check in the future if that card has been used
  int tempVal = random() % 52;
  if(tempCardList[tempVal] == -1){
    tempVal = getCardVal(tempCardList);
  }
  tempCardList[tempVal] = -1;
  return tempVal;
}

char *strlwr(char *str){
  //converts string to lowercase to be compared with strcmp()
  unsigned char *p = (unsigned char *)str;
  while (*p) {
     *p = tolower((unsigned char)*p);
      p++;
  }
  return str;
}

char *inputString(FILE* fp, size_t size){
  //takes a user input of unknown length
  //taken from https://stackoverflow.com/questions/16870485/how-can-i-read-an-input-string-of-unknown-length
    char *str;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(char)*size);
    if(!str)return str;
    while(EOF!=(ch=fgetc(fp)) && ch != '\n'){
        str[len++]=ch;
        if(len==size){
            str = realloc(str, sizeof(char)*(size+=16));
            if(!str)return str;
        }
    }
    str[len++]='\0';
    return realloc(str, sizeof(char)*len);
}

int getSum(int * array, int * tempSum){
  //gets the sum of player and dealer hands and the soft sum of the dealer
  int sum = 0;
  for(int i = 0; i < 10; i++){
    sum += array[i];
  }
  for(int i = 0; i < 10; i++){
    if(array[i] == 11 && sum > 21){
      sum -= 10;
      break;
    }
  }
  if(tempSum){
    *tempSum = 0;
    for(int i = 0; i < 10; i++){
      if(array[i] == 11)  *tempSum += 1;
      else  *tempSum += array[i];
    }
  }
  return sum;
}

int main(){
  //better than srand(time(NULL))
  //taken from https://stackoverflow.com/questions/18254325/random-number-generator-in-c
  struct timeval tm;
  gettimeofday(&tm, NULL);
  srandom(tm.tv_sec + tm.tv_usec * 1000000ul);

  int playerSum, dealerSum, cards[52];
  //generates the deck
  for(int i = 0; i <= 51; i++){
    cards[i] = i;
  }

  //used to compute the sum of the hands
  int playerHand[] = {0,0,0,0,0,0,0,0,0,0}, dealerHand[] = {0,0,0,0,0,0,0,0,0,0};

  //Starting moves
  printf("\nWelcome to blackjack!\n\nThe dealer has dealt you: \n");
  int value = getCardVal(cards);
  playerHand[0] = getNumericaVal(value);
  printf("%s%s%s%s\n", "1) ", getCardName(value), " of ", getCardSuit(value));
  value = getCardVal(cards);
  playerHand[1] = getNumericaVal(value);
  printf("%s%s%s%s\n\n", "2) ", getCardName(value), " of ", getCardSuit(value));
  playerSum = getSum(playerHand, NULL);

  int hardSum;
  value = getCardVal(cards);
  dealerHand[0] = getNumericaVal(value);
  printf("The dealer dealt themself:\n%s%s%s%s\n", "1) ", getCardName(value), " of ", getCardSuit(value));
  value = getCardVal(cards);
  int dealerHiddenCard = value;
  dealerHand[1] = getNumericaVal(dealerHiddenCard);
  dealerSum = getSum(dealerHand, &hardSum);
  printf("2) This card is hidden\n");

  printf("\nYour sum is %d.\n", playerSum);

  //intitial checks for a blackjack
  if(playerSum == 21){
    printf("\nThe dealer's hidden card is: %s%s%s\n", getCardName(dealerHiddenCard), " of ", getCardSuit(dealerHiddenCard));
    printf("The dealer's sum is: %d\n", dealerSum);
    if(dealerSum == 21){
      printf("\nDraw!\n");
      return 0;
    }
    else{
      printf("\nBlackjack! You win!\n");
      return 0;
    }
  }
  if(dealerSum == 21){
    printf("Dealer's hidden card was: %s of %s\n", getCardName(dealerHiddenCard), getCardSuit(dealerHiddenCard));
    printf("The dealer's sum is: %d\n", dealerSum);
    printf("Blackjack! Dealer wins!\n");
    return 0;
  }

  //allowing the user to input hit or stand, get new cards and check for sums over 21
  printf("\nHit or stand?\n>>");
  char * userInput;
  int k = 1;
  userInput = inputString(stdin, 10);
  while(strcmp(strlwr(userInput), "stand") != 0 && playerSum < 21){
    if(strcmp(strlwr(userInput), "hit") == 0){
      value = getCardVal(cards);
      k++;
      playerHand[k] = getNumericaVal(value);
      playerSum = getSum(playerHand, NULL);
      printf("\nYour new card is: %s%s%s\n", getCardName(value), " of ", getCardSuit(value));
      printf("Your new sum is: %d\n", playerSum);
      if(playerSum > 21){
        printf("\nDealer wins!\n");
        return 0;
      }
      printf("\nHit or stand?\n>>");
      scanf("%s", userInput);
    }
    else{
      //when the user enters something besides hit or stand
      printf("Please enter 'Hit' or 'Stand'\n>>");
      scanf("%s", userInput);
    }
  }
  free(userInput);
  printf("\n");
  printf("Dealer's hidden card is: %s of %s\n", getCardName(dealerHiddenCard), getCardSuit(dealerHiddenCard));
  printf("The dealer's sum is: %d\n\n", dealerSum);

  //the dealer's play, gets sum while below 21 or hardsum below 17
  //or dealer sum is below or equal to the player sum
  int l = 1;
  while(l){
  if(dealerSum == 21 || (dealerSum > playerSum && dealerSum < 21) || hardSum > playerSum || hardSum > 16 || (dealerSum > 20 && hardSum > 16)) break;
    value = getCardVal(cards);
    l++;
    dealerHand[l] = getNumericaVal(value);
    printf("The dealer dealt themself: %s%s%s\n", getCardName(value), " of ", getCardSuit(value));
    dealerSum = getSum(dealerHand, &hardSum);
    if(dealerSum > 21){
      printf("The dealer's hard sum is: %d\n\n", hardSum);
    }
    else{
      printf("The dealer's sum is: %d\n\n", dealerSum);
    }
  }

  //finshing checks for who won
  if(hardSum > 21 || (dealerSum > 21 && hardSum < playerSum)){
    printf("You win!\n");
    return 0;
  }
  if(dealerSum > playerSum || hardSum > playerSum){
    printf("Dealer wins!\n");
    return 0;
  }
  if(dealerSum == playerSum || hardSum == playerSum){
    printf("Draw!\n");
    return 0;
  }
  else{
    printf("You win!\n");
    return 0;
  }
}
