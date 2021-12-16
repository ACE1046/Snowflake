#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // radial to decart
    for (int i=0; i<NUM_LEDS; i++)
    {
        led_coord_x[i]= round(sin(2*Pi*led_coord_a[i]/256)*led_coord_r[i]/2)+127;
        led_coord_y[i]=-round(cos(2*Pi*led_coord_a[i]/256)*led_coord_r[i]/2)+127;
    }
    // decart to radial
//    for (int i=0; i<NUM_LEDS; i++)
//    {
//        int x, y;
//        x=led_coord_x[i];
//        y=led_coord_y[i];
//        x-=128;
//        y-=128;
//        led_coord_r[i] = round(sqrt(x*x + y*y));
//        if (x == 0) (y<0 ? led_coord_a[i]=128 : led_coord_a[i] = 0);
//        else
//            led_coord_a[i] = round(atan((double)y/x)*256/Pi+128);
//    }
    sc=new QGraphicsScene;
    ui->graphicsView->setScene(sc);
    //ui->gv->scene()->addSimpleText("test");
    n=0;
    f=0;
    last=0;
    loop_start=0;
    effect_start1=0;
    effect_start2=0;
    effect1_param=0;
    effect2_param=0;
    effect1=15;
    effect2=2;
    save_frames=0;
    p1=p2=GetPalette(0);
    ticks.start();
    srand(QDateTime::currentMSecsSinceEpoch());
}

MainWindow::~MainWindow()
{
  delete ui;
}


CHSV frame[2][NUM_LEDS];
CRGB leds[NUM_LEDS];

void AnimFlagBG(QImage *img, int phase, const palette *p, int param)
{
    int s, f, x, y;
    CHSV hsv;
    CRGB led;
    const int wave[32]={0, 2, 4, 5, 6, 7, 7, 8, 8, 8, 7, 7, 6, 5, 4, 2, 0, -2, -4, -5, -6, -7, -7, -8, -8, -8, -7, -7, -6, -5, -4, -2};
    param=0;
    for (x=0; x<256; x++)
        for (y=0; y<256; y++)
        {
            f=y + wave[(x/16+phase/48) % 32]*3;
            if (f<0) f=0;
            if (f>255) f=255;
            if ((param/2)%2) f=255-f;
            s=(f*6)/256;
            //f=(f*5)%256;
            f=0;
            ColorBlend((*p)[s%6], (*p)[(s+1)%6], f, hsv);
            hsv.v /= 2;
            hsv2rgb_rainbow(hsv, led);
            img->setPixel(x, y, qRgb(led.r, led.g, led.b));
        }
}

int ph=0;
int eff=0, pal=0, par=0;

const uint32_t time_effect=5000; // one effect duration in ms
const uint32_t time_fade=3000; // fade duration between effects in ms
const uint32_t time_loop=1800; // effect loops every X ms

#define SAVE_FRAME_MS (1000/20) // 20 fps
#define SAVE_FRAMES (time_loop / SAVE_FRAME_MS)
void MainWindow::draw_effect()
{
    int i;
    QImage image(256, 256, QImage::Format_RGB32);
    image.fill(Qt::black);

    st=ticks.elapsed();
    while (st-loop_start >= time_loop) { loop_start+=time_loop; f++;}
    n=((st-loop_start)*256*6)/time_loop;

    if (ui->cbShuffle->checkState() && st-effect_start1 >= time_effect+time_fade && effect_start2==0)
    { // fade start
        do
        {
            pal=rand()%13;
            p2 = GetPalette(pal);
        }	while (p1==p2);
        do
        {
            effect2=rand() % EFFECTS;
        }	while (effect1==effect2);
        if (effect2 == 8 && (rand()%3) == 0) { pal=9; p2=GetPalette(9); }
        effect2_param=rand();
        ui->comboBox->setCurrentIndex(effect2);
        ui->comboBox_2->setCurrentIndex(pal);
        DecodePalette();
        ui->checkBox_1->setChecked(effect2_param/1%2);
        ui->checkBox_2->setChecked(effect2_param/2%2);
        ui->checkBox_3->setChecked(effect2_param/4%2);
        ui->checkBox_4->setChecked(effect2_param/8%2);
        ui->checkBox_5->setChecked(effect2_param/16%2);
        ui->checkBox_6->setChecked(effect2_param/32%2);
        ui->checkBox_7->setChecked(effect2_param/64%2);
        effect_start2=effect_start1+time_effect+time_fade;
    }
    if (st-effect_start2 >= time_fade && effect_start2!=0)
    { // fade ended
        effect1=effect2;
        p1=p2;
        effect1_param=effect2_param;
        effect_start1=effect_start2;
        for (i=0; i<NUM_LEDS; i++)
        {
            frame[0][i][0]=frame[1][i][0];
            frame[0][i][1]=frame[1][i][1];
            frame[0][i][2]=frame[1][i][2];
        }
        effect_start2=0;
    }

    if (f==2) f=0;
    if (effect_start2>0)
    {
        Anim(effect1, n, *p1, frame[0], effect1_param);
        Anim(effect2, n, *p2, frame[1], effect2_param);
        for (i=0; i<NUM_LEDS; i++)
        {
            int p=(st-effect_start2)*255 / time_fade;
            CHSV c;
            ColorBlend(frame[0][i], frame[1][i], p, c);
            hsv2rgb_screen(c, leds[i]);
        }
    }
    else
    {
        Anim(effect1, n, *p1, frame[0], effect1_param);
        for (i=0; i<NUM_LEDS; i++)
            hsv2rgb_screen(frame[0][i], leds[i]);
    }




    //AnimFlagBG(&image, ph, GetPalette(9), 0);
    //  Anim(eff, ph, *GetPalette(pal), frame[0], par);
    //  ph+=48;
    //  if (ph>=256*6) ph-=256*6;


    QPainter p;
    QBrush b;
    p.begin(&image);
    p.setPen(Qt::transparent);
    b.setStyle(Qt::SolidPattern);
    for (int i=0; i<NUM_LEDS; i++)
    {
        b.setColor(QColor(leds[i].r, leds[i].g, leds[i].b));
        p.setBrush(b);
        p.drawEllipse(QPoint(led_coord_x[i], led_coord_y[i]), 4, 4);
    }
    p.end();

    //image.setPixel(1, 1, qRgb(255, 0, 0));
    sc->clear();
    sc->addPixmap(QPixmap::fromImage(image));

    if (save_frames && last_save-st >= SAVE_FRAME_MS)
    {
        last_save += SAVE_FRAME_MS;
        image.save(QStringLiteral("image_%1.png").arg(SAVE_FRAMES - save_frames));
        save_frames--;
    }
}

void MainWindow::on_comboBox_activated(int index)
{
    effect2 = index;
    effect2_param=par;
    effect_start2=ticks.elapsed();
}

void setLabelColor(const TPalette* p, int i, QLabel* lbl)
{
  CRGB rgb;
  hsv2rgb_screen((*p)[i], rgb);
  lbl->setStyleSheet("QLabel { background-color :"+((QVariant)QColor(rgb.r, rgb.g, rgb.b)).toString()+"; }");
}

void MainWindow::on_comboBox_2_activated(int index)
{
    pal = index;
    p2 = GetPalette(pal);
    DecodePalette();
    effect2_param=par;
    effect_start2=ticks.elapsed();
}

void MainWindow::DecodePalette()
{
    QPalette palette;
    const TPalette *p;
    p = GetPalette(pal);
    setLabelColor(p, 0, ui->label_1);
    setLabelColor(p, 1, ui->label_2);
    setLabelColor(p, 2, ui->label_3);
    setLabelColor(p, 3, ui->label_4);
    setLabelColor(p, 4, ui->label_5);
    setLabelColor(p, 5, ui->label_6);
}

void MainWindow::showEvent(QShowEvent *ev)
{
    QMainWindow::showEvent(ev);
    if (!this->timer1)
    {
        this->timer1 = new QTimer(this);
        connect(this->timer1, &QTimer::timeout, this, QOverload<>::of(&this->draw_effect));
        timer1->start(30);
        on_comboBox_2_activated(ui->comboBox_2->currentIndex());
    }
}

void MainWindow::ParamChanged()
{
    par = 0;
    if (ui->checkBox_1->checkState()) par += 1;
    if (ui->checkBox_2->checkState()) par += 2;
    if (ui->checkBox_3->checkState()) par += 4;
    if (ui->checkBox_4->checkState()) par += 8;
    if (ui->checkBox_5->checkState()) par += 16;
    if (ui->checkBox_6->checkState()) par += 32;
    if (ui->checkBox_7->checkState()) par += 64;
    if (!ui->cbShuffle->checkState()) effect1_param = par;
}

void MainWindow::on_checkBox_1_toggled(bool ) { ParamChanged(); }
void MainWindow::on_checkBox_2_toggled(bool ) { ParamChanged(); }
void MainWindow::on_checkBox_3_toggled(bool ) { ParamChanged(); }
void MainWindow::on_checkBox_4_toggled(bool ) { ParamChanged(); }
void MainWindow::on_checkBox_5_toggled(bool ) { ParamChanged(); }
void MainWindow::on_checkBox_6_toggled(bool ) { ParamChanged(); }
void MainWindow::on_checkBox_7_toggled(bool ) { ParamChanged(); }

void MainWindow::on_cbShuffle_toggled(bool checked)
{
    if (checked) effect_start1 = ticks.elapsed();
}

void MainWindow::on_pushButton_clicked()
{
    save_frames = SAVE_FRAMES;
    last_save = ticks.elapsed();
}
