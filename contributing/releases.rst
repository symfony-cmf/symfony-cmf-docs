Release Process
===============

This document explains the release process for the Symfony Content Management
Framework. See the core documentation for the `core Symfony release process`_.

Symfony CMF manages its releases through a *time-based model*; a new Symfony
CMF release comes out every *six months*. We want to synchronize with the
core Symfony release dates and release a version about one month after Symfony.
This should lead to releases in *June* and *December*.

This release cycle is for the **``symfony-cmf/symfony-cmf`` repository** and the
**symfony-cmf Standard Edition**. The individual CMF bundles and components may
release minor versions more often, if needed. ``symfony-cmf/symfony-cmf`` will
always point to a working combination and only integrate newer minor versions
when a minor release of the CMF is scheduled.

Point releases (i.e. ``1.0.1``) are used to quickly provide important fixes.
New features are never added in the point releases, but only in minor releases.
With the release of 1.0, we will create a branch 1.0 to maintain such fixes,
and master becomes aliased to 1.1.x-dev.

The CMF Standard Edition and symfony-cmf will get point releases whenever one
of the included bundles does a point release.

.. note::

    The CMF is quite new. As with Symfony 2.0, we don't want to promise BC
    at all cost yet. If reasonably doable, we will keep the code BC or use
    deprecations, but there might be exceptions where BC is too cumbersome.
    The ``UPGRADE.md`` document will help you in those cases.

Development
-----------

The six-months period is divided into two phases:

* **Development**: *Four months* to add new features and to enhance existing
  ones;

* **Stabilisation**: *Two months* to fix bugs and prepare the release.

During the development phase, any new feature can be reverted if it won't be
finished in time or if it won't be stable enough to be included in the current
final release.

Maintenance
-----------

The CMF is a community effort and as such, no maintenance can be guaranteed.
If you need maintenance contracts, please get in contact with the lead
developers. They work at internet agencies and will be able to offer support
and maintenance contracts.

Backward Compatibility
----------------------

For the next releases, we will maintain BC if possible. If something needs to
be broken, the UPGRADE.md file will help you to update the project. We will
switch to a BC-at-all-cost model later, once the CMF has stabilized and
matured.

.. note::

    The work on Symfony CMF 2.0 will start whenever enough major features breaking
    backward compatibility are waiting on the todo-list.

Deprecations
------------

When a feature implementation cannot be replaced with a better one without
breaking backward compatibility, there is still the possibility to deprecate
the old implementation and add a new preferred one along side. Read the
`conventions`_ document to learn more about how deprecations are handled in
Symfony.

Rationale
---------

This release process was adopted to give more *predictability* and
*transparency*. It is heavily inspired by the `core Symfony release process`_.

.. _Git repository: https://github.com/symfony/symfony
.. _SensioLabs: http://sensiolabs.com/
.. _core Symfony release process: https://symfony.com/doc/current/contributing/community/releases.html
.. _conventions: https://symfony.com/doc/current/contributing/code/conventions.html#contributing-code-conventions-deprecations
