
//使用说明:l298Deal(1,"加速")
void l298Deal(vu8 uart,vu8 *aa)
{
	if(aa=="前进")
		sendstr(uart,"102");
	if(aa=="后退")
		sendstr(uart,"103");
	if(aa=="左转")
		sendstr(uart,"104");
	if(aa=="右转")
		sendstr(uart,"105");
	if(aa=="停止")
		sendstr(uart,"106");
	if(aa=="加速")
		sendstr(uart,"107");
	if(aa=="减速")
		sendstr(uart,"108");
	if(aa=="不颠倒")
		sendstr(uart,"109");
	if(aa=="颠倒")
		sendstr(uart,"10A");	
	
}

