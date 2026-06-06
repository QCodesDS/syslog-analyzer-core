#ifndef ALERTNOTIFIER_H
#define ALERTNOTIFIER_H

#include "../core/Log.h"

class AlertNotifier {
public:
    static void notify(const Log& log, int count);
    static void notifyCritical(const Log& log);
};

#endif  // ALERTNOTIFIER_H