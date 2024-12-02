from PySide2.QtCore import QCoreApplication, QEventLoop, QEvent

def wait_until(callback, *, timeout=5000):
        """
        Wait in a busy loop, calling the given callback periodically until timeout is reached.

        ``callback()`` should raise ``AssertionError`` to indicate that the desired condition
        has not yet been reached, or just return ``None`` when it does. Useful to ``assert`` until
        some condition is satisfied:

        .. code-block:: python

            def view_updated():
                assert view_model.count() > 10


            qtbot.waitUntil(view_updated)

        Another possibility is for ``callback()`` to return ``True`` when the desired condition
        is met, ``False`` otherwise. Useful specially with ``lambda`` for terser code, but keep
        in mind that the error message in those cases is usually not very useful because it is
        not using an ``assert`` expression.

        .. code-block:: python

            qtbot.waitUntil(lambda: view_model.count() > 10)

        Note that this usage only accepts returning actual ``True`` and ``False`` values,
        so returning an empty list to express "falseness" raises a ``ValueError``.

        :param callback: callable that will be called periodically.
        :param timeout: timeout value in ms.
        :raises ValueError: if the return value from the callback is anything other than ``None``,
            ``True`` or ``False``.

        .. note:: This method is also available as ``wait_until`` (pep-8 alias)
        """
        __tracebackhide__ = True
        import time

        start = time.time()

        def timed_out():
            elapsed = time.time() - start
            elapsed_ms = elapsed * 1000
            return elapsed_ms > timeout

        timeout_msg = f"wait_until timed out in {timeout} milliseconds"

        while True:
            try:
                result = callback()
            except AssertionError as e:
                if timed_out():
                    raise TimeoutError(timeout_msg) from e
            else:
                if result not in (None, True, False):
                    msg = "wait_until() callback must return None, True or False, returned %r"
                    raise ValueError(msg % result)
        
                # 'assert' form
                if result is None:
                    return
        
                # 'True/False' form
                if result:
                    return
                if timed_out():
                    raise TimeoutError(timeout_msg)
        
            # While waiting, events will be processed and your test will stay
            # responsive to user interface events or network communication.
            QCoreApplication.processEvents( flags = QEventLoop.AllEvents );
            QCoreApplication.sendPostedEvents( receiver=None, event_type=QEvent.DeferredDelete );
            time.sleep(0.01)

