vu16 datw[100];
vu8 sendwificnt=0;
void wifi_start(vu8 uart)
{
    sendwificnt=0;
}
void wifi_sendnum(vu8 uart,vu32 value)
{
		if(sendwificnt)
			    datw[sendwificnt++]=',';

    if(value>=10000)
        datw[sendwificnt++]=value/10000+48;

    if(value>=1000)
        datw[sendwificnt++]=value/1000%10+48;
    if(value>=100)
        datw[sendwificnt++]=value/100%10+48;
    if(value>=10)
        datw[sendwificnt++]=value/10%10+48;
    datw[sendwificnt++]=value%10+48;

}

void wifi_sendstr(vu8 uart,vu8 *p)
{
			if(sendwificnt)
			    datw[sendwificnt++]=',';
    while(*p)
    {
        datw[sendwificnt++]=*p++;
    }
 

}

void wifi_end(vu8 uart,vu8 *num)
{
    vu8 i;
    sendchar(uart,'S');
	 sendstr(uart,num);
    sendchar(uart,sendwificnt/100+48);
    sendchar(uart,sendwificnt/10%10+48);
    sendchar(uart,sendwificnt%10+48);
    for(i=0; i<sendwificnt; i++)
    {
        sendchar(uart,datw[i]);
    }
    sendchar(uart,'E');
}





