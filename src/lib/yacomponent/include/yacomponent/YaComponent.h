#pragma once

#include <QtCore/QCoreApplication>
#include <QtCore/QDeadlineTimer>
#include <thread>
#include <chrono>

namespace YaComponent
{
constexpr int KeySize = 4;
constexpr int MessageSize = 8;
constexpr const char* KeyFmt = "%4d";
constexpr const char* MessageSizeFmt = "%8d";
constexpr int MaxMessageSize = 20 * 1024;
constexpr int MaxIdentSize = 512;
constexpr const char* SynAck = "SYNC_ACK";
constexpr int KeySync = -2;
constexpr int KeyEnd = -3;
constexpr int KeySetNotification = -4;
constexpr int KeyClearNotification = -5;
constexpr int TimeOut = 20;
constexpr int RequestResponseTimeoutMs = 5000;
void* context_new();
void context_term( void* context );
template <typename Functor> constexpr bool waitFor( Functor predicate, int timeout = RequestResponseTimeoutMs )
{
  if ( predicate() )
    return true;

  // qWait() is expected to spin the event loop, even when called with a small
  // timeout like 1ms, so we we can't use a simple while-loop here based on
  // the deadline timer not having timed out. Use do-while instead.

  int remaining = timeout;
  QDeadlineTimer deadline( remaining, Qt::PreciseTimer );

  do
  {
    // We explicitly do not pass the remaining time to processEvents, as
    // that would keep spinning processEvents for the whole duration if
    // new events were posted as part of processing events, and we need
    // to return back to this function to check the predicate between
    // each pass of processEvents. Our own timer will take care of the
    // timeout.
    QCoreApplication::processEvents( QEventLoop::AllEvents );
    QCoreApplication::sendPostedEvents( nullptr, QEvent::DeferredDelete );

    remaining = deadline.remainingTime();
    if ( remaining > 0 )
      std::this_thread::sleep_for( std::chrono::milliseconds( qMin( 10, remaining ) ) );

    if ( predicate() )
      return true;

    remaining = deadline.remainingTime();
  } while ( remaining > 0 );

  return predicate(); // Last chance
}

} // namespace YaComponent
