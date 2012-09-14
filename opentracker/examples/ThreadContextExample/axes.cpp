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
/** Axes Class implementation
 *
 * @author Alexander Bornik
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#include "axes.h"
#include <cmath>

Axes::Axes(QGraphicsItem * parent, double adimension) : QGraphicsItem(parent)
{
    setAcceptedMouseButtons(Qt::NoButton);
    d = adimension;
    a = M_PI/4;
    h = 0.1;
    setZValue(parent->zValue()+0.1);
}


QRectF Axes::boundingRect() const
{
    double a = d*(1.0-h*cos(a));
    double b = d*h*sin(a);
    QRectF rect(-b, -b,
                d+b, d+b);
    rect.adjust(-0.1, -0.1, 0.1, 0.1);
    return rect;
}


void Axes::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget)
{
    QFont font;
    font.setPointSizeF(d*0.3);
    painter->setFont(font);

    double a = d*(1.0-h*cos(a));
    double b = d*h*sin(a);

    painter->setPen(Qt::blue);
    painter->drawLine(0.0, 0.0, d, 0.0);
    painter->drawLine(QPointF(a, -b), QPointF(d, 0.0));
    painter->drawLine(QPointF(a, b), QPointF(d, 0.0));
    painter->drawText(QPointF(0.8*d, 0.3*d), "x" );

    painter->setPen(Qt::red);
    painter->drawLine(0,   0,   0, d);
    painter->drawLine(QPointF(-b,  a), QPointF(0, d));
    painter->drawLine(QPointF(b,  a), QPointF(0, d));
    painter->drawText(QPointF(0.1*d, 0.96*d), "z");
}

/* 
 * ------------------------------------------------------------
 *   End of axes.cpp
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
