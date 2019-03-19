/*
 * =====================================================================================
 *
 *       Filename:  GLCheck.cpp
 *
 *    Description:
 *
 *        Created:  27/02/2019 01:42:04
 *
 *         Author:  SFML
 *
 * =====================================================================================
 */
#include "gk/core/Debug.hpp"
#include "gk/gl/GLCheck.hpp"
#include "gk/gl/OpenGL.hpp"

namespace gk {

namespace priv {

void glCheckError(const char* file, unsigned int line, const char* expression)
{
	// Get the last error
	GLenum errorCode = glGetError();

	if (errorCode != GL_NO_ERROR)
	{
		std::string fileString = file;
		std::string error = "Unknown error";
		std::string description  = "No description";

		// Decode the error code
		switch (errorCode)
		{
			case GL_INVALID_ENUM:
			{
				error = "GL_INVALID_ENUM";
				description = "An unacceptable value has been specified for an enumerated argument.";
				break;
			}

			case GL_INVALID_VALUE:
			{
				error = "GL_INVALID_VALUE";
				description = "A numeric argument is out of range.";
				break;
			}

			case GL_INVALID_OPERATION:
			{
				error = "GL_INVALID_OPERATION";
				description = "The specified operation is not allowed in the current state.";
				break;
			}

			case GL_STACK_OVERFLOW:
			{
				error = "GL_STACK_OVERFLOW";
				description = "This command would cause a stack overflow.";
				break;
			}

			case GL_STACK_UNDERFLOW:
			{
				error = "GL_STACK_UNDERFLOW";
				description = "This command would cause a stack underflow.";
				break;
			}

			case GL_OUT_OF_MEMORY:
			{
				error = "GL_OUT_OF_MEMORY";
				description = "There is not enough memory left to execute the command.";
				break;
			}

			default:
			{
				error = "GK_UNKNOWN_ERROR";
				description = "Unknown error: " + std::to_string(errorCode);
				break;
			}

			// case GLEXT_GL_INVALID_FRAMEBUFFER_OPERATION:
			// {
			//     error = "GL_INVALID_FRAMEBUFFER_OPERATION";
			//     description = "The object bound to FRAMEBUFFER_BINDING is not \"framebuffer complete\".";
			//     break;
			// }
		}

		// Log the error
		std::cerr << "An internal OpenGL call failed in "
			<< fileString.substr(fileString.find_last_of("\\/") + 1) << "(" << line << ")."
			<< "\nExpression:\n   " << expression
			<< "\nError description:\n   " << error << "\n   " << description << "\n"
			<< std::endl;
	}
}

} // namespace priv

} // namespace gk
