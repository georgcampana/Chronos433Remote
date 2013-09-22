// *************************************************************************************************
//
//      Copyright (C) 2012 Georg Campana
//
//        THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//        "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//        LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//        OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//        SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//        LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//        DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//        THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//        OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// *************************************************************************************************

#ifndef HOME_H_
#define HOME_H_

// *************************************************************************************************
// Include section

// *************************************************************************************************
// Prototypes section



// *************************************************************************************************
// Defines section

// *************************************************************************************************
// Global Variable section


// *************************************************************************************************
// Extern section

extern void mx_hremote(u8 line);
extern void sx_hremote(u8 line);
extern void display_hremote(u8 line, u8 update);

extern void mx_hgate(u8 line);
extern void sx_hgate(u8 line);
extern void display_hgate(u8 line, u8 update);

#endif                          /*HOME_H_ */
