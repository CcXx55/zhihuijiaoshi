void OLED_DisStra(vu8 uart, u8 Line, u8 Row, u8 *Dat)
{
    vu8 oleda;
    if (Line == 0)
        sendstr(uart, "h0 l0=");
    if (Line == 1)
        sendstr(uart, "h1 l1=");
    if (Line == 2)
        sendstr(uart, "h2 l2=");
    if (Line == 3)
        sendstr(uart, "h3 l3=");
    oleda = Row * 8;
    sendchar(uart, oleda / 10 + 48);
    sendchar(uart, oleda % 10 + 48);
    if (Line == 0)
        sendstr(uart, " oled0=");
    if (Line == 1)
        sendstr(uart, " oled1=");
    if (Line == 2)
        sendstr(uart, " oled2=");
    if (Line == 3)
        sendstr(uart, " oled3=");
    sendstr(uart, Dat);
}
