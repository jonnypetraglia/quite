#include "image_manager.h"
#include "main_window.h"

ImageManager::ImageManager(MainWindow* window)
{
    main_window = window;
    image_widget = new QLabel;
    image_widget->setStyleSheet("QWidget { background-color: black; }");
    QSizePolicy policy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    policy.setHorizontalStretch(0);
    policy.setVerticalStretch(0);
    image_widget->setSizePolicy(policy);
    image_widget->setMinimumHeight(100);
    image_widget->setMinimumHeight(200);
    image_widget->setAlignment(Qt::AlignCenter);
    image_widget->setMovie(new QMovie);
    image_widget->movie()->setScaledSize(QSize(100,100));

    FILETYPES << "*.jpg" << "*.png" << "*.gif";
    initChecks();

    QMainWindow::connect(image_widget->movie(), SIGNAL(frameChanged(int)), (ImageManager*)this, SLOT(movieFinished(int)));
}

void ImageManager::load(QString file)
{
    main_window->setCentralWidget(image_widget);
    image_widget->movie()->stop();

    qDebug() << "Showing File " << file;

    image_widget->movie()->setFileName(file);

    img_size = QPixmap(file).size();
    QSize newsize = img_size;
    newsize.scale(main_window->size().width()-100, main_window->size().height()-100, Qt::KeepAspectRatio);
    image_widget->movie()->setScaledSize(newsize);

    movie_is_finished = image_widget->movie()->frameCount()==1;
    setSpeed(100);
}

void ImageManager::setSpeed(double speedFrom0To100)
{
    qDebug() << "speed " << speedFrom0To100;
    image_widget->movie()->setSpeed(speedFrom0To100);
    image_widget->movie()->start();
}

void ImageManager::faster()
{
    double speed = getSpeed();
    if(speed > 100)
    {
        if(speed >= 1000)
            speed += 1000;
        else
            speed += 100;
    } else {
        if(speed >= 60)
            speed += 10;
        else
            speed += 5;
    }
    main_window->setSpeed(speed);
}

void ImageManager::slower()
{
    double speed = getSpeed();
    if(speed > 100) {
        if(speed >= 1000)
            speed -= 500;
        else
            speed -= 50;
    }else {
        if(speed > 60)
            speed -= 5;
        else if(speed > 5)
            speed -= 2.5;
        else
            return;
    }
    main_window->setSpeed(speed);
}

bool ImageManager::togglePause()
{
    image_widget->movie()->setPaused(image_widget->movie()->state()!=QMovie::Paused);
    return image_widget->movie()->state()==QMovie::Paused;
}

void ImageManager::resize(QResizeEvent *qre)
{
    QSize newsize = img_size;
    newsize.scale(qre->size().width(), qre->size().height(), Qt::KeepAspectRatio);
    image_widget->movie()->setScaledSize(newsize);
    image_widget->movie()->start();
}

void ImageManager::next()
{
    if(movie_is_finished)
        main_window->nextItem();
    else
        main_window->nextItemWhenConvenient();
    qDebug() << "nextSlide";
}

void ImageManager::movieFinished(int frame)
{
    if(frame == image_widget->movie()->frameCount()-1) {
        qDebug() << "Frame #" << frame;
        movie_is_finished = true;
        main_window->nextItemWhenConvenient();
    }
}
