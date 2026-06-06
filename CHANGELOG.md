# Changelog

Формат основан на [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
версии следуют [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- Настройки `pre-commit` для запуска `clang-format`.
- Руководство для контрибьюторов.
- Проверка Valgrind в CI.

### Changed

- Санитайзеры CMake и Makefile теперь включают AddressSanitizer и
  UndefinedBehaviorSanitizer.

## [1.0.1] - 2026-06-02

### Fixed

- Исправлена конфигурация Dependabot.

## [1.0.0] - 2026-05-29

### Added

- Консольный архиватор на основе алгоритма Хаффмана.
- Сжатие и распаковка текстовых и бинарных файлов.
- Модульные, интеграционные и стресс-тесты.
- CMake-сборка, GitHub Actions CI, CodeQL и Dependabot.
