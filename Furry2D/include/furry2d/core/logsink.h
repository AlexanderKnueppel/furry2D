#ifndef __FURRY_CORE_LOGSINK_H__
#define __FURRY_CORE_LOGSINK_H__  

/*
* ****************************************
*
* This file is part of Furry2D, a simple gameframework for 2D desktop games.
*
* Copyright (c) 2015 Furry2D. All rights reserved.
*
* For the full copyright and license information, please view the LICENSE.txt
* file that was distributed with this source code.
*
* \Author Alexander Knueppel
*
* ****************************************
*/

FURRY_NS_BEGIN

/**
* \brief Log sink
*
* \ingroup core
*/
class FURRY_API LogSink {
public:
	/**
	The wrapped type T should have:
	- void operator()(const LogMessage& meta, const std::string& message) const;
	*/
	template <typename T>
	LogSink(T impl) : wrapper_(new Model<T>(std::forward<T>(impl))) {}

	LogSink(const LogSink& sink);
	//LogSink(LogSink&&) noexcept = default; //problem with vs2013?

	LogSink& operator= (const LogSink& sink) FURRY_NOEXCEPT;
	//LogSink& operator= (LogSink&&) noexcept = default; //problem with vs2013?

	bool operator == (const LogSink& sink) const;

	void forward(
		const LogMessage::Meta& meta,
		const std::string& message
		) const;

private:
	struct Concept {
		virtual ~Concept() = default;
		virtual Concept* clone() const = 0;

		virtual void forward(
			const LogMessage::Meta& meta,
			const std::string& message
			) const = 0;
	};

	template <typename T>
	struct Model : Concept {
		Model(T impl) : impl_(std::forward<T>(impl)) {}

		virtual Concept* clone() const  override {
			return new Model<T>(impl_);
		}

		virtual void forward(
			const LogMessage::Meta& meta,
			const std::string& message
			) const override {
			impl_(meta, message);
		}

		T impl_;
	};

	std::unique_ptr<const Concept> wrapper_;
};

LogSink FURRY_API makeConsoleSink();
LogSink FURRY_API makeFileSink(const std::string& filename);

FURRY_NS_END

#endif