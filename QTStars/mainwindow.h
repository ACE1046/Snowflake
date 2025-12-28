#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "effects.h"
#include <QElapsedTimer>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void draw_effect();

  void on_comboBox_activated(int index);

  void on_comboBox_2_activated(int index);

  void on_checkBox_1_toggled(bool checked);
  void on_checkBox_2_toggled(bool checked);
  void on_checkBox_3_toggled(bool checked);
  void on_checkBox_4_toggled(bool checked);
  void on_checkBox_5_toggled(bool checked);
  void on_checkBox_6_toggled(bool checked);
  void on_checkBox_7_toggled(bool checked);

  void on_cbShuffle_toggled(bool checked);

  void on_pushButton_clicked();

  void on_comboBox_activated(const QString &arg1);

protected:
  void showEvent(QShowEvent *ev);

private:
  uint32_t last, st, loop_start, effect_start1, effect_start2, f, n, last_save;
  int effect1, effect2, effect1_param, effect2_param;
  int save_frames;
  const TPalette *p1, *p2;
  Ui::MainWindow *ui;
  QTimer *timer1 = NULL;
  QGraphicsScene *sc;
  QElapsedTimer ticks;
  void ParamChanged();
  void DecodePalette();
};

#endif // MAINWINDOW_H
