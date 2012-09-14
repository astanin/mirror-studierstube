/* ========================================================================
 * Copyright (c) 2006,
 * Institute for Computer Graphics and Vision
 * Graz University of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Graz University of Technology nor the names of
 * its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ========================================================================
 * PROJECT: OpenTracker
 * ======================================================================== */
/** Puck Class implementation
 *
 * @author Alexander Bornik
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#include "ponghelp.h"
#include "puck.h"
#include "playingfield.h"
#include "arena.h"
#include "paddle.h"
#include "dilatedrect.h"

Puck::Puck(double adimension, PlayingField * parea):
    QObject(),
    QGraphicsItem(),
    Poseable(),
    color(Qt::white)
{
    timer = new QTimer;
    dimension = adimension;
    setZValue(9.0);
    t1 = QTime::currentTime();
    dx = 6.0;
    dz = -5.0;
    playarea = parea;
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timerEvent()));
    timer->start(1000.0/33.0);
            
}

void Puck::addPaddle(Paddle * xpaddle)
{
    paddles.append(xpaddle);
}

QPainterPath Puck::shape() const
{
    QPainterPath path;
    path.addEllipse(QRectF(-0.5*dimension, -0.5*dimension, dimension, dimension));
    return path;
}

QRectF Puck::boundingRect() const
{
    return QRectF(-dimension*0.5, -dimension*0.5,
                  dimension, dimension);
}

QPointF Puck::process()
{
    QTime ts = QTime::currentTime();
    double Dx = (dx*(t1.msecsTo(t2)/1000.0));
    double Dz = (dz*(t1.msecsTo(t2)/1000.0));
    t1 = t2;
    QMatrix m = QGraphicsItem::matrix();
    return QPointF(m.dx() + Dx, m.dy() + Dz);
}

void Puck::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget)
{
    painter->setPen(Qt::black);
    painter->setBrush(color);
    painter->drawEllipse(boundingRect());
}

void Puck::timerEvent()
{
    QPointF new_pos(process());
    new_pos = testForWallCollisions(new_pos);
    new_pos = testForPaddleCollisions(new_pos);

    double theta = atan2(-dx, dz);
    //setPose(new_pos, theta);
}

QPointF Puck::testForPaddleCollisions(const QPointF &pos)
{
    using namespace std;
    double epsilon = 1.0; // give the puck a little bounce away from the object so it doesn't get stuck
    QPointF v(dx, dz);
    
    QList<Paddle*>::iterator it;
    
    for (it = paddles.begin(); it != paddles.end(); it++)
    {
        QPointF r((*it)->getPosition() - getPosition());
        if (length(r) < (dimension*0.5 + (*it)->dimension*0.5))
        {
            QPointF n(-normalised(r));// normal vector
            QPointF t(n.y(), -n.x());   // tangent vector
	    cerr <<  length(n) << ", " << length(t) << endl;
            double vn(-(dot(v, n)));
            double vt(dot(v, t));
            QPointF vprime(vn*n.x(), vn*n.y());        
            vprime += QPointF(vt*t.x(), vt*t.y());
            cerr << "speed = " << length(vprime) << endl;
            dx = vprime.x();
            dz = vprime.y();
            
            QPointF nudge = normalised(vprime);
            nudge *= epsilon;

            return (nudge + pos);
        }
    }
    return pos;
}
    
QPointF Puck::testForWallCollisions(const QPointF &pos)
{
    QRectF rect(playarea->inner_bound->boundingRect());
    double epsilon = 0.2; // give the puck a little bounce away from the object so it doesn't get stuck
    QPointF nudge;
    double x = pos.x();
    double z = pos.y();
    if (x > rect.right())
    {
        dx = -dx;
        nudge += QPointF(-epsilon, 0);
        //self.translate(-epsilon, 0)
    }
    else if (x < rect.left())
    {
        dx = -dx;
        //self.translate(epsilon, 0)
        nudge += QPointF(epsilon, 0);
    }
    if (z > rect.bottom())
    {
        dz = -dz;
        //self.translate(0.0, -epsilon)
        nudge += QPointF(0, -epsilon);
    }
    else if (z < rect.top())
    {
        dz = -dz;
        nudge += QPointF(0, epsilon);
        //#self.translate(0.0, epsilon)
    }
    return (nudge + pos);
}


/* 
 * ------------------------------------------------------------
 *   End of paddle.cpp
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
